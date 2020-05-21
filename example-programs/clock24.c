/**
 * @file clock24.c
 *
 * An example program demonstrating a simple real-time task: a clock.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/clock_24.h>
#include <Lazuli/serial.h>

#include <Lazuli/sys/arch/AVR/usart.h>

DEPENDENCY_ON_MODULE(CLOCK_24);
DEPENDENCY_ON_MODULE(SERIAL);

/* This array could also be stored in ROM using PROGMEM. */
static const char * const printableValues[] =
  {
   "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
   "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
   "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
   "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
   "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
   "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
  };

void
ClockTask(void)
{
  Clock24 clock24;

  for (;;) {
    Lz_Task_WaitActivation();

    Lz_Clock24_Get(&clock24);

    Usart_PrintRawString(printableValues[clock24.hours]);
    Usart_PutChar(':');
    Usart_PrintRawString(printableValues[clock24.minutes]);
    Usart_PutChar(':');
    Usart_PrintRawString(printableValues[clock24.seconds]);
    Usart_NewLine();
  }
}

static void
EnableSerialTransmission(void) {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_9600;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

/**
 * Main entry point for user tasks.
 */
int
main(void)
{
  Lz_TaskConfiguration taskConfiguration;

  EnableSerialTransmission();

  Usart_PrintRawString("\r\n>>>>>>>\r\n");

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.schedulingPolicy = CYCLIC_RT;
  taskConfiguration.period = 50;
  taskConfiguration.completion = 25;
  Lz_RegisterTask(ClockTask, &taskConfiguration);

  Lz_Run();

  for (;;);
}
