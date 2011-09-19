#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "strstring.h"
#include "fstring.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main (int argc, char **argv)
{
  int f;
  struct stat s;
  fstring fs;

  if (argc != 4)
    {
      fprintf(stderr, "usage: %s [file] [regex match] [replace]\n",  argv[0]);
      return 1;
    }
 
  if ((f = open(argv[1], O_RDONLY)) == -1)
    {
      perror("could not load");
      return 1;
    }

  if (fstat(f, &s))
    {
      perror("cannot fstat");
      return 1;
    }

  fs = fsnnew("", s.st_size);
  read(f, fs.str, s.st_size);
  close(f);

  fsreplace(&fs, argv[2], argv[3], FS_REPLACE_ALL);

  write(fileno(stdout), fs.str, fs.len);

  return 0;
}

