#include <stdio.h>
#include <string.h>
#include "fobj.h"
#include "simpleobj.h"

int main(int argc, char **argv)
{  
  newObj(Person, Dan);
  $(Dan, setname, "Silly Simon");

  newObj(Person, Ron);
  $(Ron, setname, "Ronald Regane");

  $(Dan, print);
  $(Ron, print);
  $(Dan, setname, $(Ron, getname));

  $(Dan, print);

  free(Dan);

  return 0;
}


