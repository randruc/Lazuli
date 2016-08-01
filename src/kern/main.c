/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <sys/registers.h>

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

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  unsigned int i;
  
  DDRB = (char)0xff;

  for (i = 0; i < 60000; i++);
  
  PrintInt(0x5555);
  
  while (1);
}
