/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <sys/registers.h>
#include <sys/arch.h>

void
BlinkOnce()
{
  unsigned int i;
  
  PORTB = (char)0xff;
  for (i = 0; i < 20000; i++);
  PORTB = (char)0;
  for (i = 0; i < 60000; i++);
}

void
BlinkTwice()
{
  unsigned int i;
  
  PORTB = (char)0xff;
  for (i = 0; i < 20000; i++);
  PORTB = (char)0;
  for (i = 0; i < 10000; i++);
  PORTB = (char)0xff;
  for (i = 0; i < 20000; i++);
  PORTB = (char)0;
  for (i = 0; i < 60000; i++);
}

void
PrintInt(unsigned int i)
{
  unsigned char j;

  for (j = 0; j < sizeof(i) * 8; j++) {
	if ((i & 1) > 0) {
	  BlinkOnce();
	} else {
	  BlinkTwice();
	}

	i = i >> 1;
  }
}

unsigned volatile int mode;

void
Int0Handler()
{
  mode = !mode;
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  unsigned int i, t;
  mode = 0;

  DDRB = (char)0xff;
  EICRA = (char)0x03;
  EIMSK = (char)0x01;

  GlobalInterruptsEnable();
  
  t = 4;

  while (t--) {
	PORTB = (char)0xff;
	for (i = 0; i < 5000; i++);
	PORTB = (char)0x00;
	for (i = 0; i < 5000; i++);
  }
  
  while (1) {
	if (mode) {
	  t = 10000;
	} else {
	  t = 5000;
	}

	PORTB = (char)0xff;
	for (i = 0; i < t; i++);
	PORTB = (char)0x00;
	for (i = 0; i < t; i++);
  }
  
  while (1);
}
