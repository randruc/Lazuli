/**
 * @file src/kern/mutex_2.c
 * @brief Mutexes demonstration program.
 * @date Mar 2019
 * @author Remi Andruccioli
 *
 * Example program to demonstrate the use of mutexes in Lazuli.
 *
 * This program satisfies the following specification:
 *
 *  "Write a multithreaded program that prints a sequence of characters like
 *   this: A, B, A, B, A, B...
 *   Synchronize properly so it never prints the same character more than once
 *   in a row."
 *
 * It is written after this blog post:
 * https://blog.vorbrodt.me/2019/02/10/interview-question-part-1/
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/mutex.h>
#include <Lazuli/serial.h>

#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/usart.h>

/*
 * Define the number of iterations for each loop.
 */
static const uint8_t LOOP_N = 5;

/*
 * Define the mutexes associated with each task.
 */
static Lz_Mutex mutexA = LZ_MUTEX_INIT_LOCKED;
static Lz_Mutex mutexB = LZ_MUTEX_INIT_LOCKED;

static void
TaskA(void)
{
  uint8_t i;

  for (i = 0; i < LOOP_N; ++i) {
    Lz_Mutex_Lock(&mutexA);
    Usart_PrintRawString("A\r\n");
    Lz_Mutex_Unlock(&mutexB);
  }
}

static void
TaskB(void)
{
  uint8_t i;

  Lz_Mutex_Unlock(&mutexA);
  
  for (i = 0; i < LOOP_N; ++i) {
    Lz_Mutex_Lock(&mutexB);
    Usart_PrintRawString("B\r\n");
    Lz_Mutex_Unlock(&mutexA);
  }
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
  EnableSerialTransmission();

  Lz_SetSchedulerClass(LZ_SCHED_RR);

  Lz_RegisterTask(TaskA, NULL);
  Lz_RegisterTask(TaskB, NULL);

  Usart_PrintRawString("\r\n");

  Lz_Run();

  for (;;);
}
