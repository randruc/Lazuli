/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <sys/arch/AVR/registers.h>
#include <sys/arch/AVR/arch.h>
#include <sys/arch/AVR/timer0.h>
#include <sys/types.h>

static volatile bool mode;

void
Int0Handler()
{
  mode = !mode;
  
  if (mode) {
	PORTB = (char)0xff;
  } else {
	PORTB = (char)0x00;
  }
}

void
Timer0CompareMacthAHandler()
{
  mode = !mode;
  
  if (mode) {
	PORTB = u8_MAX;
  } else {
	PORTB = u8_MIN;
  }
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  /* unsigned int i; */
  mode = false;

  DDRB = (u8)0xff;
  EICRA = (u8)0x03;
  EIMSK = (u8)0x01;

  Timer0SelectClock(SystemClock);
  Timer0SetMode(CTC);
  Timer0SetCompareRegisterA((u8)20);
  Timer0Enable();
  Timer0OutputCompareMatchAInterruptEnable();

  GlobalInterruptsEnable();

  /* 
   * while (t--) {
   * 	PORTB = (char)0xff;
   * 	for (i = 0; i < 5000; i++);
   * 	PORTB = (char)0x00;
   * 	for (i = 0; i < 5000; i++);
   * }
   */
  
  /* 
   * while (1) {
   * 	if (mode) {
   * 	  t = 50000;
   * 	} else {
   * 	  t = 5000;
   * 	}
   * 
   * 	PORTB = (char)0xff;
   * 	for (i = 0; i < t; i++);
   * 	PORTB = (char)0x00;
   * 	for (i = 0; i < t; i++);
   * }
   */
  
  while (1);
}
