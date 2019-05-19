/**
 * @file src/kern/spinlocks.c
 * @brief Spinlocks demonstration program.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * Example program to demonstrate the use of spinlocks in Lazuli.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>
#include <Lazuli/spinlock.h>

#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/usart.h>

static Lz_Spinlock lock = LZ_SPINLOCK_INIT;

void
Task()
{
  Lz_Spinlock_Lock(&lock);
  Usart_PrintRawString(Lz_Task_GetName());
  Lz_Spinlock_Unlock(&lock);
}

static void
EnableSerialTransmission() {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

void
main(void)
{
  Lz_TaskConfiguration taskConfiguration;

  EnableSerialTransmission();

  Lz_SetSchedulerClass(LZ_SCHED_RR);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 1 says: "
    "\"An ounce of prevention is better than a pound of cure.\"\r\n";
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 2 says: "
    "\"Don't count your chickens before they hatch.\"\r\n";
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 3 says: "
    "\"Beauty is in the eye of the beholder.\"\r\n";
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 4 says: "
    "\"Fire is a good servant but a bad master.\"\r\n";
  Lz_RegisterTask(Task, &taskConfiguration);

  EICRA = 0x0f;
  EIMSK = 0x03;

  Usart_PrintRawString("\r\n");

  Lz_Run();

  for (;;);
}
