#ifndef HEADER_F94E88F4203549FF9348C20B1A8E40B3
#define HEADER_F94E88F4203549FF9348C20B1A8E40B3

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "conf.h"
#include "6502.h"

class Trace;
class TraceEventType;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <shared/enum_decl.h>
#include "6522.inl"
#include <shared/enum_end.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// NOTE: On big-endian architectures (e.g. WiiU/PPC), GCC allocates bitfields
// from the MSB first. All bitfield structs below have their field order
// reversed under CPU_BIG_ENDIAN so that named fields map to the correct
// hardware register bit positions on both platforms.
// Hardware bit layout: bit 0 = LSB, bit 7 = MSB of the uint8_t value.

class R6522 {
  public:
#include <shared/pushwarn_bitfields.h>
    struct PCRBits {
#if CPU_BIG_ENDIAN
        uint8_t cb2_mode : 3;
        uint8_t cb1_pos_irq : 1;
        uint8_t ca2_mode : 3;
        uint8_t ca1_pos_irq : 1;
#else
        uint8_t ca1_pos_irq : 1;
        uint8_t ca2_mode : 3;
        uint8_t cb1_pos_irq : 1;
        uint8_t cb2_mode : 3;
#endif
    };
#include <shared/popwarn.h>

    union PCR {
        PCRBits bits;
        uint8_t value;
    };

#include <shared/pushwarn_bitfields.h>
    struct ACRBits {
#if CPU_BIG_ENDIAN
        uint8_t t1_output_pb7 : 1;
        uint8_t t1_continuous : 1;
        uint8_t t2_count_pb6 : 1;
        uint8_t sr : 3;
        uint8_t pb_latching : 1;
        uint8_t pa_latching : 1;
#else
        uint8_t pa_latching : 1;
        uint8_t pb_latching : 1;
        uint8_t sr : 3;
        uint8_t t2_count_pb6 : 1;
        uint8_t t1_continuous : 1;
        uint8_t t1_output_pb7 : 1;
#endif
    };
#include <shared/popwarn.h>

    union ACR {
        ACRBits bits;
        uint8_t value;
    };
    typedef union R6522ACR R6522ACR;

#include <shared/pushwarn_bitfields.h>
    struct IRQBits {
#if CPU_BIG_ENDIAN
        uint8_t _ : 1;
        uint8_t t1 : 1;
        uint8_t t2 : 1;
        uint8_t cb1 : 1;
        uint8_t cb2 : 1;
        uint8_t sr : 1;
        uint8_t ca1 : 1;
        uint8_t ca2 : 1;
#else
        uint8_t ca2 : 1;
        uint8_t ca1 : 1;
        uint8_t sr : 1;
        uint8_t cb2 : 1;
        uint8_t cb1 : 1;
        uint8_t t2 : 1;
        uint8_t t1 : 1;
        uint8_t _ : 1;
#endif
    };
#include <shared/popwarn.h>

    union IRQ {
        uint8_t value;
        IRQBits bits;
    };

#if BBCMICRO_TRACE
#include <shared/pshpack1.h>
    struct IRQEvent {
        uint8_t id;
        IRQ ifr, ier;
    };
#include <shared/poppack.h>

#include <shared/pshpack1.h>
    struct TimerTickEvent {
        uint16_t new_t1;
        uint16_t new_t2;
        uint8_t id : 4;
        uint8_t t1_ticked : 1;
        uint8_t t2_ticked : 1;
    };
#include <shared/poppack.h>

    static const TraceEventType IRQ_EVENT;
    static const TraceEventType TIMER_TICK_EVENT;
#endif

    /* Cx1 and Cx2 */
#include <shared/pushwarn_bitfields.h>
    struct Port {
      private:
        uint8_t or_ = 0;
        uint8_t ddr = 0;

      public:
        uint8_t p = 0xff;

      private:
        uint8_t p_latch = 0xff;

      public:
        uint8_t c1 = 0;

      private:
        uint8_t old_c1 = 0;

      public:
        uint8_t c2 = 0;

      private:
        uint8_t old_c2 = 0;
        uint8_t pulse = 0;

        friend class R6522;
#if BBCMICRO_DEBUGGER
        friend class R6522DebugWindow;
#endif
    };
#include <shared/popwarn.h>

    Port a = {};
    Port b = {};
    IRQ ifr = {};
    IRQ ier = {};

    void Reset();

    void SetID(uint8_t id, const char *name);

    static void Write0(void *via, M6502Word addr, uint8_t value);
    static void Write1(void *via, M6502Word addr, uint8_t value);
    static void Write2(void *via, M6502Word addr, uint8_t value);
    static void Write3(void *via, M6502Word addr, uint8_t value);
    static void Write4(void *via, M6502Word addr, uint8_t value);
    static void Write5(void *via, M6502Word addr, uint8_t value);
    static void Write6(void *via, M6502Word addr, uint8_t value);
    static void Write7(void *via, M6502Word addr, uint8_t value);
    static void Write8(void *via, M6502Word addr, uint8_t value);
    static void Write9(void *via, M6502Word addr, uint8_t value);
    static void WriteA(void *via, M6502Word addr, uint8_t value);
    static void WriteB(void *via, M6502Word addr, uint8_t value);
    static void WriteC(void *via, M6502Word addr, uint8_t value);
    static void WriteD(void *via, M6502Word addr, uint8_t value);
    static void WriteE(void *via, M6502Word addr, uint8_t value);
    static void WriteF(void *via, M6502Word addr, uint8_t value);

    static uint8_t Read0(void *via, M6502Word addr);
    static uint8_t Read1(void *via, M6502Word addr);
    static uint8_t Read2(void *via, M6502Word addr);
    static uint8_t Read3(void *via, M6502Word addr);
    static uint8_t Read4(void *via, M6502Word addr);
    static uint8_t Read5(void *via, M6502Word addr);
    static uint8_t Read6(void *via, M6502Word addr);
    static uint8_t Read7(void *via, M6502Word addr);
    static uint8_t Read8(void *via, M6502Word addr);
    static uint8_t Read9(void *via, M6502Word addr);
    static uint8_t ReadA(void *via, M6502Word addr);
    static uint8_t ReadB(void *via, M6502Word addr);
    static uint8_t ReadC(void *via, M6502Word addr);
    static uint8_t ReadD(void *via, M6502Word addr);
    static uint8_t ReadE(void *via, M6502Word addr);
    static uint8_t ReadF(void *via, M6502Word addr);

    PCR GetPCR() const;

    uint8_t UpdatePhi2LeadingEdge();
    uint8_t UpdatePhi2TrailingEdge();

#if BBCMICRO_TRACE
    void SetTrace(Trace *t, bool extra);
#endif
  protected:
  private:
    uint8_t m_t1ll = 250;
    uint8_t m_t1lh = 202;
    uint8_t m_t2ll = 0;
    uint8_t m_t2lh = 0;
    uint8_t m_sr = 0;
    ACR m_acr = {};
    PCR m_pcr = {};

    uint16_t m_t1 = 0;
    bool m_t1_reload = false;
    bool m_t1_pending = false;
    bool m_t1_timeout = false;

    uint16_t m_t2 = 0;
    bool m_t2_reload = false;
    bool m_t2_pending = false;
    bool m_t2_timeout = false;
    bool m_t2_count = true;

    uint8_t m_t1_pb7 = 0;
    uint8_t m_old_pb = 0;

#if BBCMICRO_TRACE
    Trace *m_trace = nullptr;
    bool m_trace_extra = false;
#endif

    uint8_t m_id = 0;
    const char *m_name = nullptr;

    void TickControlPhi2TrailingEdge(Port *port,
                                     uint8_t latching,
                                     uint8_t pcr_bits,
                                     uint8_t cx2_mask,
                                     char c);

#if BBCMICRO_DEBUGGER
    friend class R6522DebugWindow;
#endif
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CHECK_SIZEOF(R6522::PCR, 1);
CHECK_SIZEOF(R6522::ACR, 1);
CHECK_SIZEOF(R6522::IRQ, 1);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
