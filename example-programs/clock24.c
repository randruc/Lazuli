/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>
#include <Lazuli/clock_24.h>

#include <Lazuli/sys/arch/AVR/usart.h>

void
Task1()
{
  Clock24 clock24;

  for (;;) {
    Lz_Task_WaitActivation();

    Lz_Clock24_Get(&clock24);
    
    Usart_HexaPrint_u16(clock24.hours);
    Usart_PutChar(':');
    Usart_HexaPrint_u16(clock24.minutes);
    Usart_PutChar(':');
    Usart_HexaPrint_u16(clock24.seconds);
    Usart_NewLine();
  }
}

static void
EnableSerialTransmission() {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

/**
 * Main entry point for user tasks.
 */
void
main(void)
{
  Lz_TaskConfiguration taskConfiguration;

  EnableSerialTransmission();

  Usart_PrintRawString("\r\n>>>>>>>\r\n");

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.period = 50;
  taskConfiguration.completion = 5;
  Lz_RegisterTask(Task1, &taskConfiguration);
  
  Lz_Run();

  for (;;);
}
