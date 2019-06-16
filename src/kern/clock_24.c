
#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/clock_24.h>
#include <Lazuli/sys/clock_24.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/memory.h>

/**
 * The system Clock24.
 */
static volatile Clock24 clock24 = {0};

/**
 * Increment an uint8_t value, and reset it if it reaches the value in
 * @p comparator.
 *
 * @param value A valid pointer to the value to increment.
 * @param comparator The value used for comparison.
 *
 * @return _true_ if:
 *           - The value was reset
 *         _false_ if:
 *           - The value was incremented without resetting
 */
static bool
IncrementUntil(volatile uint8_t * const value, const uint8_t comparator)
{
  if (*value == comparator) {
    *value = 0;

    return true;
  }

  (*value)++;

  return false;
}

/**
 * Increment the seconds of the system 24-Hour clock.
 *
 * @return _true_ if the minutes need to be incremented, else _false_.
 */
static bool
IncrementSeconds(void)
{
  return IncrementUntil(&clock24.seconds, 59);
}

/**
 * Increment the minutes of the system 24-Hour clock.
 *
 * @return _true_ if the hours need to be incremented, else _false_.
 */
static bool
IncrementMinutes(void)
{
  return IncrementUntil(&clock24.minutes, 59);
}

/**
 * Increment the hours of the system 24-Hour clock.
 */
static void
IncrementHours(void)
{
  IncrementUntil(&clock24.hours, 24);
}

void
Clock24_Increment(void)
{
  const uint16_t ticksInOneSecond = LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY;
  static uint16_t ticksToNewSecond = 0;

  ticksToNewSecond++;

  if (ticksToNewSecond < ticksInOneSecond) {
    return;
  }

  ticksToNewSecond = 0;

  if (clock24.version == UINT8_MAX) {
    clock24.version = 0;
  } else {
    clock24.version++;
  }

  if (IncrementSeconds()) {
    if (IncrementMinutes()) {
      IncrementHours();
    }
  }
}

/*
 * The working of this function needs an explanation.
 *
 * While we read all the fields of struct Clock24 clock24, the kernel can
 * interrupt this function to update the fields. If that happends we will
 * obtain a copy of struct Clock24 clock24 that is not correct. i.e. half of the
 * fields with the previous value, and the other half with the updated value.
 *
 * To avoid that, we use this kind of "optimistic access":
 * A version number is contained in struct Clock24. As all of the fields are
 * updated by the kernel at the same time without interrupting, the new values
 * of the time fields in the struct with correspond to a new value of the
 * version field.
 * We first read the current version number, then read all the time fields, and
 * finally read the version number again. If the two version numbers differ we
 * then know that the kernel updated the struct Clock24 clock24 while we were
 * accessing it.
 */
void
Lz_Clock24_Get(Clock24 * const userClock24)
{
  uint8_t version;

  if (NULL == userClock24) {
    return;
  }

  do {
    /* We don't use Memory_Copy() here to guarantee the order of copying. */
    version = clock24.version;
    userClock24->hours = clock24.hours;
    userClock24->minutes = clock24.minutes;
    userClock24->seconds = clock24.seconds;
    userClock24->version = clock24.version;
  } while (version != userClock24->version);
}
