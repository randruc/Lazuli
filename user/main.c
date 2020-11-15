/**
 * Main user file boilerplate.
 *
 * Start writing your program here.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

/**
 * Define a task to be scheduled in the system.
 */
void
Task1(void)
{
  for (;;) {
    /*
     * You code goes here.
     */
  }
}

/**
 * Main entry point for the user program.
 */
void
main(void)
{
  /* Register the defined task to run */
  Lz_RegisterTask(Task1, NULL);

  /* Run the system */
  Lz_Run();

  /* This will never be reached, this is to conform to ISO C main() prototype */
  return 0;
}
