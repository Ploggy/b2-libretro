// Creates a 24 bpp R8_G8_B8 surface. This format coexists nicely with
// stbi_image_write, which is a bit inflexible in terms of input format.
//
// Output formats can be:
//
// - SDL_PIXELFORMAT_RGB24 = R8G8B8 - for stb_image 3-component RGB writing
// - SDL_PIXELFORMAT_XRGB8888 = B8G8R8X8 (LE) / X8R8G8B8 (BE) - for Windows clipboard
// - SDL_PIXELFORMAT_BGR24 = B8G8R8 - for Windows clipboard, 24 bpp opaque bitmap

SDLUniquePtr<SDL_Surface> BeebWindow::CreateScreenshot(SDL_PixelFormatEnum pixel_format) const {
    ASSERT(pixel_format == SDL_PIXELFORMAT_RGB24 ||
           pixel_format == SDL_PIXELFORMAT_BGR24 ||
           pixel_format == SDL_PIXELFORMAT_XRGB8888);
    UniqueLock<Mutex> lock;
    uint32_t *tv_pixels;
    if (m_settings.screenshot_last_vsync) {
        tv_pixels = m_tv.GetLastVSyncTexturePixels(&lock);
    } else {
        tv_pixels = m_tv.GetTexturePixels(nullptr);
    }

    // Endian-aware surface creation
    // The TVOutput stores pixels in native-endian format determined by GetTexelValue():
    // - Little-endian systems: BGRA (B at byte 0, G at byte 1, R at byte 2, X at byte 3)
    // - Big-endian systems: XRGB (X at byte 0, R at byte 1, G at byte 2, B at byte 3)
#if CPU_LITTLE_ENDIAN
    const uint32_t rmask = 0x00ff0000;  // R at byte 2
    const uint32_t gmask = 0x0000ff00;  // G at byte 1
    const uint32_t bmask = 0x000000ff;  // B at byte 0
    const uint32_t amask = 0x00000000;  // No alpha
#else
    const uint32_t rmask = 0x00ff0000;  // R at byte 1 (in BE: offset 1)
    const uint32_t gmask = 0x0000ff00;  // G at byte 2 (in BE: offset 2)
    const uint32_t bmask = 0x000000ff;  // B at byte 3 (in BE: offset 3)
    const uint32_t amask = 0x00000000;  // No alpha
#endif

    // temporary surface referring to the 32 bpp pixel data in the TVOutput object.
    SDLUniquePtr<SDL_Surface> src_surface(SDL_CreateRGBSurfaceFrom(tv_pixels,
                                                                   TV_TEXTURE_WIDTH, TV_TEXTURE_HEIGHT,
                                                                   32,
                                                                   TV_TEXTURE_WIDTH * 4,
                                                                   rmask,
                                                                   gmask,
                                                                   bmask,
                                                                   amask));

    if (m_settings.screenshot_correct_aspect_ratio) {
        SDLUniquePtr<SDL_Surface> surface(SDL_CreateRGBSurface(0,
                                                               int(TV_TEXTURE_WIDTH * CORRECT_ASPECT_RATIO_X_SCALE), TV_TEXTURE_HEIGHT,
                                                               32,
                                                               src_surface->format->Rmask,
                                                               src_surface->format->Gmask,
                                                               src_surface->format->Bmask,
                                                               src_surface->format->Amask));
        int blit_result;
        if (m_settings.screenshot_filter) {
            blit_result =
#if HAVE_SDL_SOFTSTRETCHLINEAR
                SDL_SoftStretchLinear
#else
                SDL_BlitScaled
#endif
                (src_surface.get(), nullptr, surface.get(), nullptr);
        } else {
            blit_result = SDL_BlitScaled(src_surface.get(), nullptr, surface.get(), nullptr);
        }

        if (blit_result != 0) {
            m_msg.e.f("Failed to resize image: %s\n", SDL_GetError());
            return nullptr;
        }

        src_surface = std::move(surface);
    }

    if (src_surface->format->format == (Uint32)pixel_format) {
        return src_surface;
    } else {
        std::unique_ptr<SDL_Surface, SDL_Deleter> surface(SDL_CreateRGBSurfaceWithFormat(0, src_surface->w, src_surface->h, 24, pixel_format));
        if (SDL_BlitSurface(src_surface.get(), nullptr, surface.get(), nullptr) != 0) {
            m_msg.e.f("Failed to copy image: %s\n", SDL_GetError());
            return nullptr;
        }

        return surface;
    }
}
