#include <stdio.h>
#include <stdlib.h>
#include "strstring.h"
#include "fstring.h"
#include "fstack.h"

int
main (int argc, char **argv)
{
  fstack stacky;
  fstack_init(&stacky);

  int i;
  for (i = 1; i <= 20000; i++)
    {
      int *x = (int *) malloc (sizeof(int));
      *x = i;
      fstack_insert(&stacky, i/2, "wisdom");
    }

  //fstack_insert(&stacky, 2, "dope");

  //puts((char*) fstack_delete(&stacky, 3));

  //puts((char *)fstack_shift(&stacky));
  //puts((char *)fstack_pop(&stacky));

  printf("array length: %i\n", fstack_length(&stacky));

  //  puts((char *)fpop(&stacky));

  

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

  fstring *test2 = fsnewp("indeed");
  printf("FS: %s\n", test2->str);  
  fsfreep(test2);

  return 0;
}

