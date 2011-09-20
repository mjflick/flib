#include <stdio.h>
#include <stdlib.h>

static char *
_fatal (char *string)
{
  fprintf(stderr, "%s\n", string);
  exit(EXIT_FAILURE);
}

void *
xcalloc (size_t lsize, size_t size)
{
  register void *value = calloc (lsize, size);
  if (value == 0)
    _fatal ("virtual memory exhausted");
  return value;
}

void *
xrealloc (void *ptr, size_t size)
{
  ptr = realloc(ptr, size);

  if (ptr == 0)
    _fatal ("virtual memory exhausted");
  return ptr;
}

void *
xmalloc (size_t size)
{
  register void *value = malloc (size);
  if (value == 0)
    _fatal ("virtual memory exhausted");
  return value;
}

