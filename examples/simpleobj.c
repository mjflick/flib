#include <stdio.h>
#include <string.h>
#include "fobj.h"
#include "simpleobj.h"

int main(int argc, char **argv)
{  
  Person *Dan = newObj(Person);
  $(Dan, setname, "Silly Simon");

  Person *DanCopy = cloneObj(Person, Dan);

  Person *Ron = newObj(Person);
  $(Ron, setname, "Ronaldo");
  $(Ron, print);
  $(Dan, print);

  $(Dan, setname, $(Ron, getname));
  $(Dan, print);

  $(DanCopy, print);

  printf("At the end of the day, the winner is %s\n", $(Dan, getname));
  printf("Runner up is %s\n", DanCopy->name);

  //  free(Dan);

  return 0;
}


