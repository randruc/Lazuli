# C style and conventions

This file describes the programming style and conventions to apply on C code.

As a general rule, the code must be clear and readable. Reading it must be a
pleasant experience.  
The code must not contain useless or redundant things.


## C dialect

The whole project is written in pure ANSI C 89.
All C code files are encoded in raw 7-bit ASCII.

> This is to the allow C code to be ported easily to many platforms and
> architectures, by being compiled by the largest number of compilers.
> Another reason is that C89 is understood by all C code-checking tools.


## Line length

Line length is set to 80 characters. Each line of code shouldn't exceed 80
columns.

> Documents must be easily readable without horizontal scrolling in a console
> (e.g. with `less` or `cat`), in simple text editors, or when comparing two
> versions in a file diff utility.


## Comments

Only use C89 comments, no C99/C++ comments.

> This is to be compliant with C89.

Never write nested comments. Never write "pretty typos".
Never comment out code unless you want to document something.

Use only one of the 2 following forms.
1-line comments can use both allowed forms, while multiple line comments must
use the second form only.

This is good:
```C
/* First form. This is a comment */

/*
 * Second form.
 *
 * This is a comment
 * that spans over
 * multiple lines.
 */
```

This is not good:
```C
// This is a comment.

/* This is a comment
that spans over
multiple lines */

/*
 * This is a comment.
 * And now this is /* a nested comment */
 */
 
/***************************************************
 *         Hey, look! I'm an artist!!!!            *
 ***************************************************/
```

### Where to put comments

Good code shouldn't need comments.
Good code could should express by itself.


As a general rule:
* Avoid comments inside functions
* Only comment the API

Use comments inside functions very carefully only to explain something not
obvious for the reader.

Every function must be documented using Doxygen tags.
For static functions, this is done above the function itself.
For public functions, this is done above the function prototype in the header
file only.


### Inline documentation



## Types

The return type of a function must be on the line preceding the function name.

This is good :
```C
int
sum(int a, int b)
{
  return a + b;
}
```

This is not good :
```C
int sum(int a, int b)
{
  return a + b;
}
```

> C code must be easily readable in console or with editors that don't support
> syntax highlighting.


## Curly braces

Curly braces use the K&R style.

For functions, opening and closing braces are always on their own line.

This is good:
```C
int main(void)
{
  printf("Hello world\n");
     
  return EXIT_SUCCESS;
}
```

This is not good:
```C
int main(void) {
  printf("Hello world\n");

  return EXIT_SUCCESS;
}
    
int main(void)
{
  printf("Hello world\n");

  return EXIT_SUCCESS; }
```

For control statements the opening braces are always on the same line than the
statement and the closing braces are always on a new line.

This is good:
```C
if (n == 4) {
  printf("Bye");
  return 0;
}
```

This is not good:
```C
if (n == 4)
{
  printf("Bye");
  return 0;
}
    
if (n == 4) {
  printf("Bye");
  return 0; }
```

This applies for all control statements that involve code sections:
if, else, for, while, do.

For if-else blocks, the else keyword must be on the same line than the braces.

This is good:
```C
if (n == 4) {
  j += 8;
  printf("Bye");
} else {
  j = 0;
  i++;
}
```

This is not good:
```C
if (n == 4) {
  j += 8;
  printf("Bye");
}
else {
  j = 0;
  i++;
}

if (n == 4) {
  j += 8;
  printf("Bye");
}
else
{
  j = 0;
  i++;
}
```

For control statements that involve code section, braces must always be
present.

This is good:
```C
if (map == NULL) {
  return NULL;
}

for (i = 0; i < LENGTH; i++) {
  t[i] = NULL;
}
```

This is not good:
```C
if (map == NULL)
  return NULL;

for (i = 0; i < LENGTH; i++)
  t[i] = NULL;
```

> Always putting the braces helps to avoid some bugs.
> As an example, you can read this article about the famous case of Apple's
> SSL/TLS bug:
> https://embeddedgurus.com/barr-code/2014/03/apples-gotofail-ssl-security-bug-was-easily-preventable/

TODO: one-line ifs...


## Whitespaces, tabs and newlines

Never use tabs to indent C code. C code must use 2 spaces for indentation.

> Every editor must display the code the same way.


Lines musn't have trailing whitespaces.

Files must end with a newline character.

> The code file must print correctly when using tools like `cat` in console.

TODO: spaces before () in if, for, while
