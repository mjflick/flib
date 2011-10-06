#include <stdio.h>
#include <string.h>
#include "fobj.h"
#include "simpleobj.h"

int main(int argc, char **argv)
{  
  Person *Dan = newObj(Person);
  $(Dan, setname, "Silly Simon");

  Person *Ron = newObj(Person);
  $(Ron, setname, "Ronaldo");
  $(Ron, print);
  $(Dan, print);

  $(Dan, setname, $(Ron, getname));
  $(Dan, print);

  //  free(Dan);

  return 0;
}


