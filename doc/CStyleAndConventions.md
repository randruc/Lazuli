# C style and conventions

This file describes the programming style and conventions to apply on C code.


## C dialect

The whole project is written in pure ANSI C 89.

## Line length

Line length is set to 80 characters. Each line of code shouldn't exceed 80
columns.

## Comments

Only use C89 comments, no C/C++ comments.

This is good:

    /* This is a comment */

    /*
     * This is a comment
     */

This is not good:

    // This is a comment


## Types

The return type of a function must be on the previous line of the function name.

This is good :

    int
    sum(int a, int b)
    {
      return a + b;
    }

This is not good :

    int sum(int a, int b)
    {
      return a + b;
    }


## Brackets

Brackets use the K&R style.

For functions, the opening bracket is always on a new line and the closing
bracket is always on a new line too.

This is good:

    int main(void)
    {
      printf("Hello world\n");
     
      return EXIT_SUCCESS;
    }

This is not good:

    int main(void) {
      printf("Hello world\n");
    
      return EXIT_SUCCESS;
    }
    
    int main(void)
    {
      printf("Hello world\n");
    
      return EXIT_SUCCESS; }
    

For control statements the opening bracket is always on the same line than the
statement and the closing bracket is always on a new line.

This is good:

    if (n == 4) {
      printf("Bye");
      return 0;
    }

This is not good:

    if (n == 4)
    {
      printf("Bye");
      return 0;
    }
    
    if (n == 4) {
      printf("Bye");
      return 0; }


This applies for all control statements that involve code sections:
if, else, for, while, do

For if-else blocks, the else keyword must be on the same line than the brackets.

This is good:

    if (n == 4) {
      j += 8;
      printf("Bye");
    } else {
      j = 0;
      i++;
    }

This is not good:

    if (n == 4) {
      j += 8;
      printf("Bye");
    }
    else {
      j = 0;
      i++;
    }

For control statements that involve code section brackets must always be
present.

This is good:

    if (map == NULL) {
      return NULL;
    }

    for (i = 0; i < LENGTH; i++) {
      t[i] = NULL;
    }

This is not good:

    if (map == NULL)
      return NULL;

    for (i = 0; i < LENGTH; i++)
      t[i] = NULL;

