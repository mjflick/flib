
char*  strnew(const char *);
char*  strnnew(const char *, size_t);
char*  strset(char *, const char *);
char*  strnset(char *, const char *, size_t);
int    streq(char *, char *);
char*  strappend(char *, const char *);
char*  strnappend(char *, const char *, size_t);
int    strmatch (char *, const char *);
char*  strreplace (char *, const char *, const char *, int flags);
char*  strnreplace (char *, const char *, const char *, size_t, int flags);

extern int SS_REPLACE_ALL;


