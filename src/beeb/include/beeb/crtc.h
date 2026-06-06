#ifndef HEADER_D20E0E20832349CAA17E68B81F546B14
#define HEADER_D20E0E20832349CAA17E68B81F546B14

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "conf.h"
#include "6502.h"

class Trace;

#include <shared/enum_decl.h>
#include "crtc.inl"
#include <shared/enum_end.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// NOTE: On big-endian architectures (e.g. WiiU/PPC), GCC allocates bitfields
// from the MSB first. All bitfield structs below have their field order
// reversed under CPU_BIG_ENDIAN so that named fields map to the correct
// hardware register bit positions on both platforms.
//
// The CRTC Output struct uses uint32_t bitfields. The same reversal rule
// applies: on BE, the first declared field occupies the MSBs.
//
// Hardware bit layout for 8-bit registers: bit 0 = LSB, bit 7 = MSB.

class CRTC {
  public:
    struct Output {
#if CPU_BIG_ENDIAN
        uint32_t raster : 5;
        uint32_t address : 14;
        uint32_t cudisp : 1;
        uint32_t display : 1;
        uint32_t vsync : 1;
        uint32_t hsync : 1;
#else
        uint32_t hsync : 1;
        uint32_t vsync : 1;
        uint32_t display : 1;
        uint32_t cudisp : 1;
        uint32_t address : 14;
        uint32_t raster : 5;
#endif
    };

    static uint8_t ReadAddress(void *c_, M6502Word a);
    static void WriteAddress(void *c_, M6502Word a, uint8_t value);
    static uint8_t ReadData(void *c_, M6502Word a);
    static void WriteData(void *c_, M6502Word a, uint8_t value);

    Output Update(uint8_t lightpen);

#if BBCMICRO_TRACE
    void SetTrace(Trace *t,
                  bool trace_scanlines,
                  bool trace_scanlines_separators);
#endif
  protected:
  private:
#include <shared/pushwarn_bitfields.h>
    struct R3Bits {
#if CPU_BIG_ENDIAN
        uint8_t wv : 4;
        uint8_t wh : 4;
#else
        uint8_t wh : 4;
        uint8_t wv : 4;
#endif
    };
#include <shared/popwarn.h>

    union R3 {
        uint8_t value;
        struct R3Bits bits;
    };

#include <shared/pushwarn_bitfields.h>
    struct R8Bits {
#if CPU_BIG_ENDIAN
        uint8_t c : 2;
        uint8_t d : 2;
        uint8_t _ : 2;
        uint8_t v : 1;
        uint8_t s : 1;
#else
        uint8_t s : 1;
        uint8_t v : 1;
        uint8_t _ : 2;
        uint8_t d : 2;
        uint8_t c : 2;
#endif
    };
#include <shared/popwarn.h>

    union R8 {
        uint8_t value;
        struct R8Bits bits;
    };

#include <shared/pushwarn_bitfields.h>
    struct R10Bits {
#if CPU_BIG_ENDIAN
        uint8_t mode : 2;
        uint8_t start : 5;
#else
        uint8_t start : 5;
        uint8_t mode : 2;
#endif
    };
#include <shared/popwarn.h>

    union R10 {
        uint8_t value;
        struct R10Bits bits;
    };
    typedef union CRTCR10 CRTCR10;

    struct RegisterBits {
        uint8_t nht;
        uint8_t nhd;
        uint8_t nhsp;
        R3 nsw;
        uint8_t nvt;
        uint8_t nadj;
        uint8_t nvd;
        uint8_t nvsp;
        R8 r8;
        uint8_t nr;
        R10 ncstart;
        uint8_t ncend;
        uint8_t addrh, addrl;
        uint8_t cursorh, cursorl;
        uint8_t penh, penl;
    };

    union Registers {
        uint8_t values[18];
        RegisterBits bits;
    };

    struct InternalState {
        uint8_t column = 0;
        uint8_t row = 0;
        uint8_t raster = 0;
        int8_t vsync_counter = -1;
        int8_t hsync_counter = -1;
        int8_t vadj_counter = -1;
        bool hdisp = true;
        bool vdisp = true;
        M6502Word line_addr = {};
        M6502Word next_line_addr = {};
        M6502Word char_addr = {};
        uint32_t num_updates = 0;
        uint8_t skewed_display = 0;
        uint8_t skewed_cudisp = 0;
        bool check_vadj = false;
        bool in_vadj = false;
        bool end_of_vadj_latched = false;
        bool had_vsync_this_row = false;
        bool end_of_main_latched = false;
        bool do_even_frame_logic = false;
        bool first_scanline = false;
        bool in_dummy_raster = false;
        bool end_of_frame_latched = false;
        bool cursor = false;
        bool old_lightpen = false;
    };

    Registers m_registers = {};
    uint8_t m_address = 0;
    InternalState m_st;

    uint8_t m_num_frames = 0;

#if BBCMICRO_TRACE
    Trace *m_trace = nullptr;
    bool m_trace_scanlines = false;
    bool m_trace_scanlines_separators = false;
#endif

    void EndOfFrame();
    void EndOfRow();
    void EndOfScanline();

#if BBCMICRO_DEBUGGER
    friend class CRTCDebugWindow;
#endif
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
