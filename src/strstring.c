#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include "strstring.h"

/* define global flags */
int SS_REPLACE_ALL = 0x01;

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

char *
strnew(const char *string)
{
  return strnnew(string, strlen(string));
}


char *
strnnew(const char *string, size_t len)
{
  char *s = (char *) _xmalloc( len + 1 );
  
  memset(s, 0x0, len + 1);
  strncpy(s, string, len);

  return s;
}

char *
strset(char *s, const char *string)
{
  return strnset(s, string, strlen(string));
}

char *
strnset(char *s, const char *string, size_t len)
{
  s = (char *) _xrealloc(s, len + 1);
  
  memset(s, 0x0, len + 1);
  strncpy(s, string, len);

  return s;
}

int
streq (char *s, char *comp)
{
  int s_len = strlen(s);
  if (s_len == strlen(comp))
    {
      return(strncmp(s, comp, s_len) ? 0 : 1);
    }
  else
    {
      return 0;
    }
}

int
strmatch (char *s, const char *regex)
{
  int     result;
  regex_t re;

  if (regcomp(&re, regex, REG_EXTENDED|REG_NOSUB) != 0) return(0);

  result = regexec(&re, s, 0, 0, 0);
  regfree(&re);

  if (result != 0) return 0;

  return 1;
}

struct fsnreplacement {
  char *s;
  int success;
};

static int
_fsnreplace (struct fsnreplacement *fsnr, regex_t *re, const char *rep_str, size_t rep_len)
{
  int        result;
  regmatch_t pmatch;
  size_t     new_str_len;
  size_t     complete_str_len;
  size_t     len_wo_match;
  size_t     s_len = strlen(fsnr->s);

  result = regexec(re, fsnr->s, 1, &pmatch, 0);

  if (result != 0) return 0;

  // allocate space for new string
  new_str_len = (s_len - ( pmatch.rm_eo - pmatch.rm_so )) + rep_len;

  char *new_str = (char *) _xmalloc (new_str_len + 1);

  // zero out new string
  memset(new_str, 0x0, new_str_len + 1);

  // apply first half of string
  memcpy(new_str, fsnr->s, pmatch.rm_so);

  // apply replacement of string
  memcpy(new_str + pmatch.rm_so, rep_str, rep_len);

  // apply remaining half of string
  memcpy(new_str + pmatch.rm_so + rep_len,
         fsnr->s + pmatch.rm_eo, 
         s_len - pmatch.rm_so - (pmatch.rm_eo - pmatch.rm_so));

  free(fsnr->s);
  fsnr->s = new_str;
  return 1;
}

char *
strreplace (char *s, const char *regex, const char *rep_str, int flags)
{
  return(strnreplace(s, regex, rep_str, strlen(rep_str), flags));
}

char *
strnreplace (char *s, const char *regex, const char *rep_str, size_t rep_len, int flags)
{
  /* build regex */
  int        results = 0;
  regex_t    re;

  /* Compile regex */
  if (regcomp(&re, regex, REG_EXTENDED) != 0) return(0);

  struct fsnreplacement fsnr;
  fsnr.success = 0;
  fsnr.s       = s;

  if ((~(~SS_REPLACE_ALL | flags)) == 0)
    {
      while( _fsnreplace(&fsnr, &re, rep_str, rep_len) ) ;
    }
  else
    { 
      results = _fsnreplace(&fsnr, &re, rep_str, rep_len);
    }

  /* free regex allocation */
  regfree(&re);

  s = fsnr.s;
  return s;
}

char *
strappend(char *s, const char *string)
{
  return strnappend(s, string, strlen(string));
}

char *
strnappend(char *s, const char *string, size_t len)
{
  size_t s_len   = strlen(s);
  size_t tot_len = len + s_len;

  char *new_str = (char *) _xmalloc (tot_len + 1);
  memset(new_str, 0x0, tot_len + 1);
  memcpy(new_str, s, s_len);
  memcpy(new_str + s_len, string, len);

  free(s);
  s = new_str;

  return s;
}

