typedef struct {
  char   *str;
  size_t len;
} fstring;

fstring fsnew(const char *);
fstring fsnnew(const char *, size_t);
int     fsset(fstring *, const char *);
int     fsnset(fstring *, const char *, size_t);
int     fseq(fstring *, fstring *);
int     fsappend(fstring *, const char *);
int     fsnappend(fstring *, const char *, size_t);
int     fsmatch (fstring *f, const char *);
int     fsreplace (fstring *, const char *, const char *, int flags);
int     fsreplace (fstring *, const char *, const char *, int flags);

extern int FS_REPLACE_ALL;

//char *appbend_fstring(fstring *, const char *);

