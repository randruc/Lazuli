/**
 * @file timer0.c
 *
 * Describes the "8-bit timer/counter 0" driver for the atmega328p.
 */

#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/timer0.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/common.h>
#include <Lazuli/sys/assert.h>

void
Timer0SetMode(const Timer0Mode mode)
{
  /* TODO: Remove switch */
  switch (mode) {
  case Normal:
  case PwmPhaseCorrect0xFF:
    break;

  case CTC:
    CLEAR_BITS(TCCR0A, u8, TCCR0A_WGM00);
    SET_BITS(TCCR0A, u8, TCCR0A_WGM01);
    CLEAR_BITS(TCCR0B, u8, TCCR0B_WGM02);
    break;

  case FastPwm0xFF:
  case PWMPhaseCorrect:
  case FastPwm:
  default:
    break;
  }
}

/* TODO: Find a decent name for that type and its instances */
/* TODO: Comment that type */
/**
 * Represent a couple of values to set/clear in the TCCR0B register.
 */
typedef struct {
  const u8 toSet;   /** < Value to set in the register   */
  const u8 toClear; /** < Value to clear in the register */
}ToSetToClear;

/* TODO: Store that table in ROM */
/**
 * Table of configuration values to set/clear in the TCCR0B register, for each
 * value of Timer0ClockSelect.
 * This table MUST stay synchronized with the entries of Timer0ClockSelect.
 */
static const ToSetToClear toSetToClear[] = {
  { /* Stop */
    0,
    TCCR0B_CS02 | TCCR0B_CS01 | TCCR0B_CS00
  },

  { /* Clock */
    TCCR0B_CS00,
    TCCR0B_CS02 | TCCR0B_CS01
  },

  { /* Clock_8 */
    TCCR0B_CS01,
    TCCR0B_CS02 | TCCR0B_CS00
  },

  { /* Clock_64 */
    TCCR0B_CS01 | TCCR0B_CS00,
    TCCR0B_CS02
  },

  { /* Clock_256 */
    TCCR0B_CS02,
    TCCR0B_CS01 | TCCR0B_CS00
  },

  { /* Clock_1024 */
    TCCR0B_CS02 | TCCR0B_CS00,
    TCCR0B_CS01
  },

  { /* ExternalT0Falling */
    TCCR0B_CS02 | TCCR0B_CS01,
    TCCR0B_CS00
  },

  { /* ExternalT0Rising */
    TCCR0B_CS02 | TCCR0B_CS01 | TCCR0B_CS00,
    0
  }
};

void
Timer0SelectClock(const Timer0ClockSelect clockSelect)
{
  const ToSetToClear *t;

  DEBUG_ASSERT(clockSelect > ExternalT0Rising || clockSelect < 0);

  t = &toSetToClear[clockSelect];

  CLEAR_BITS(TCCR0B, u8, t->toClear);
  SET_BITS(TCCR0B, u8, t->toSet);
}

void
Timer0Stop()
{
  Timer0SelectClock(Stop);
}

void
Timer0SetCompareRegisterA(const u8 value)
{
  OCR0A = value;
}

void
Timer0Enable()
{
  CLEAR_BITS(PRR, u8, PRR_PRTIM0);
}

void
Timer0Disable()
{
  SET_BITS(PRR, u8, PRR_PRTIM0);
}

void
Timer0OutputCompareMatchAInterruptEnable()
{
  SET_BITS(TIMSK0, u8, TIMSK0_OCIE0A);
}

void
Timer0OutputCompareMatchAInterruptDisable()
{
  CLEAR_BITS(TIMSK0, u8, TIMSK0_OCIE0A);
}
