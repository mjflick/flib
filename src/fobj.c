#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *
_fatal (char *string)
{
  fprintf(stderr, "%s\n", string);
  exit(EXIT_FAILURE);
}

static void *
_xmalloc (size_t size)
{
  register void *value = malloc (size);
  if (value == 0)
    _fatal ("virtual memory exhausted");
  return value;
}

void *
flibnewObj (size_t s, void (*init_function)(void *))
{
  register void *obj = (void *) _xmalloc (s);

  /* run initialization function */
  init_function(obj);

  return obj;
}

void *
flibcloneObj (size_t s, void *p)
{
  register void *obj = (void *) _xmalloc (s);

  /* copy initial object */
  memcpy(obj, p, s);

  return obj;
}
