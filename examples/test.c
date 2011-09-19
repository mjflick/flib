#include <stdio.h>
#include <stdlib.h>
#include "strstring.h"
#include "fstring.h"

int
main (int argc, char **argv)
{
  // if you just want to use a char *, the strstring is for you
  char *string = strnew("wisdom is high");
  string = strreplace(string, "wisdom", "mighty", 0);
  string = strappend(string, " running: ");
  string = strappend(string, argv[0]);
  
  if (strmatch(string, "xighty")) {
    puts("pizza party for the poor");
  }

  string = strappend(string, "\ncheese");
  string = strreplace(string, "e", "xe", SS_REPLACE_ALL);

  printf("S: %s\n", string);
  free(string);


  // if you want something more sane, try fstring
  fstring test = fsnew("what a sad day.");

  // woot, a regular expression replacement
  fsreplace(&test, "\\.$", ", for those who are sad!", 0);

  if (fsmatch(&test, "!$"))
    puts("It's a happy day!");

  printf("FS: %s\n", test.str);

  fsfree(&test);

  return 0;
}

