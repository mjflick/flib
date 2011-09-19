#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include "fstring.h"

/* define global flags */
int FS_REPLACE_ALL = 0x01;

static char *
_fatal (char *string)
{
  fprintf(stderr, "%s\n", string);
  exit(EXIT_FAILURE);
}

static void *
_xcalloc (size_t lsize, size_t size)
{
  register void *value = calloc (lsize, size);
  if (value == 0)
    _fatal ("virtual memory exhausted");
  return value;
}

static void *
_xrealloc (void *ptr, size_t size)
{
  ptr = realloc(ptr, size);

  if (ptr == 0)
    _fatal ("virtual memory exhausted");
  return ptr;
}

static void *
_xmalloc (size_t size)
{
  register void *value = malloc (size);
  if (value == 0)
    _fatal ("virtual memory exhausted");
  return value;
}

fstring
fsnew(const char *string)
{
  return fsnnew(string, strlen(string));
}

fstring
fsnnew(const char *string, size_t len) 
{
  fstring new;
  new.len = len;
  new.str = (char *) _xmalloc(len + 1);
  
  memset(new.str, 0x0, new.len + 1);
  strncpy(new.str, string, new.len);

  return new;
}

void
fsfree(fstring *f)
{
  free(f->str);
}

int
fsset(fstring *f, const char *string)
{
  return fsnset(f, string, strlen(string));
}

int
fsnset(fstring *f, const char *string, size_t len)
{
  f->str = (char *) _xrealloc(f->str, len + 1);
  
  memset(f->str, 0x0, len + 1);
  strncpy(f->str, string, len);
  f->len = len;

  return 1;
}

int
fseq (fstring *a, fstring *b)
{
  if (a->len == b->len)
    {
      return(strncmp(a->str, b->str, a->len) ? 0 : 1);
    }
  else
    {
      return 0;
    }
}

int
fseqs (fstring *f, const char *str)
{
  return(fsneqs(f, str, strlen(str)));
}

int
fsneqs (fstring *f, const char *str, size_t len) 
{
  if (f->len == len) {
    return(strncmp(f->str, str, len) ? 0 : 1);
  }
  else {
    return 0;
  }
}

int
fsmatch (fstring *f, const char *regex)
{
  int     result;
  regex_t re;

  if (regcomp(&re, regex, REG_EXTENDED|REG_NOSUB) != 0) return(0);

  result = regexec(&re, f->str, 0, 0, 0);
  regfree(&re);

  if (result != 0) return 0;

  return 1;
}

static int
_fsnreplace (fstring *f, regex_t *re, const char *rep_str, size_t rep_len, int *f_rep_off)
{
  int        result;
  regmatch_t pmatch;
  size_t     new_str_len;
  size_t     complete_str_len;
  size_t     len_wo_match;

  result = regexec(re, f->str + *f_rep_off, 1, &pmatch, 0);

  if (result != 0) return 0;

  // allocate space for new string
  new_str_len = (f->len - ( pmatch.rm_eo - pmatch.rm_so )) + rep_len;

  char *new_str = (char *) _xmalloc (new_str_len + 1);

  // zero out new string
  memset(new_str, 0x0, new_str_len + 1);

  // apply first half of string
  memcpy(new_str, f->str, *f_rep_off + pmatch.rm_so);

  // apply replacement of string
  memcpy(new_str + *f_rep_off + pmatch.rm_so, rep_str, rep_len);

  // apply remaining half of string
  memcpy(new_str + pmatch.rm_so + rep_len + *f_rep_off,
         f->str  + pmatch.rm_eo + *f_rep_off, 
         f->len  - pmatch.rm_so - *f_rep_off - (pmatch.rm_eo - pmatch.rm_so));

  free(f->str);
  f->str = new_str;
  f->len = new_str_len;
  *f_rep_off = *f_rep_off + pmatch.rm_so + rep_len;

  return 1;
}

int
fsreplace (fstring *f, const char *regex, const char *rep_str, int flags)
{
  return(fsnreplace(f, regex, rep_str, strlen(rep_str), flags));
}

int
fsnreplace (fstring *f, const char *regex, const char *rep_str, size_t rep_len, int flags)
{
  /* build regex */
  int        results   = 0;
  int        f_rep_off = 0;
  regex_t    re;

  /* Compile regex */
  if (regcomp(&re, regex, REG_EXTENDED) != 0) return(0);

  if ((~(~FS_REPLACE_ALL | flags)) == 0)
    {
      while( _fsnreplace(f, &re, rep_str, rep_len, &f_rep_off) ) results++;
    }
  else
    { 
      results = _fsnreplace(f, &re, rep_str, rep_len, &f_rep_off);
    }

  /* free regex allocation */
  regfree(&re);

  return results;
}

int
fsappend(fstring *f, const char *string)
{
  return fsnappend(f, string, strlen(string));
}

int
fsnappend(fstring *f, const char *string, size_t len)
{
  size_t tot_len = len + f->len;

  char *new_str = (char *) _xmalloc (tot_len + 1);
  memset(new_str, 0x0, tot_len + 1);
  memcpy(new_str, f->str, f->len);
  memcpy(new_str + f->len, string, len);

  free(f->str);
  f->str = new_str;
  f->len = tot_len;

  return 1;
}

