#include <stdlib.h>
#include <string.h>


typedef struct fs_item {  
  void *data; 
  unsigned int id;
  struct fs_item *next;
  struct fs_item *previous;
} fhashstack_item;

typedef struct {
  fhashstack_item *first;
  fhashstack_item *last;
  int size;
} fhashstack;



typedef struct fhash_item {
  char *key;
  void *data;
  struct fhash_item *next;
} fh_item;

typedef struct {
  fh_item **data;
  int size;
} fhash;

static unsigned int
mk_hash ( const char *key )
{
  unsigned int hash = 0;

  while ( *key ) hash ^= *key++;
  return hash;
}

void *
fhash_data ( fhash *t, const char *key ) 
{
  fh_item *h;
  unsigned int pos;
 
  if ( !t || t->size <= 0 || !key )
    return NULL;
  
  pos = mk_hash(key) % t->size;

  for ( h = t->data[pos]; h != NULL; h = h->next )
    if ( key && strcmp(key, h->key) == 0 )
      return h->data;

  return NULL;
}

int
fhash_exists( fhash *t, const char *key )
{
  unsigned int index;
  fh_item *h;

  index = mk_hash(key) % t->size;

  for ( h = t->data[index]; h != NULL; h = h->next )
    if ( strcmp(key, h->key) == 0 )
      return 1;

  return 0;
}


void *
fhash_delete ( fhash *t, const char *key ) 
{
  unsigned int index;
  void *data;
  fh_item *h, *last = NULL;
  
  if ( t == NULL || t->size <= 0 || key == NULL )
    return NULL;

  index = mk_hash(key) % t->size;

  for ( h = t->data[index]; h != NULL; h = h->next ) {
    if ( strcmp(key, h->key) == 0 ) {
      if ( last )
        last->next = h->next;
      else 
        t->data[index] = h->next;
      free(h->key);
      data = h->data;
      free(h);
      h = NULL;
      return data;
    }
    last = h;
  }

  return NULL;
}


int 
fhash_free ( fhash *t ) 
{
  int i;
  fh_item *h, *next = NULL;

  if ( t == NULL || t->size <= 0 )
    return -1;

  for ( i = 0; i < t->size; i++ ) {
    for ( h = t->data[i]; h != NULL; h = next ) {
      next = h->next;
      if ( h->key )
        free(h->key); 
      free(h);
    }
  }
  free(t->data);

  t->size = 0;
  return 0;
}

int 
fhash_insert ( fhash *t, const char *key, void *data ) 
{
  unsigned int index;
  fh_item *h, *hh;
  
  if ( t == NULL || t->size <= 0 || key == NULL )
    return -1;
 
  index = mk_hash(key) % t->size;
  
  if ( t->data[index] == NULL ) { 
    t->data[index] = (fh_item *) xmalloc(sizeof(fh_item));
    
    t->data[index]->key  = strdup(key);
    if ( !t->data[index]->key )
      return -1;
    
    t->data[index]->data = data;
    t->data[index]->next = NULL;
    
    return index;
  }
  
  /* no, then find if the key already exists, and reach the last link */
  for ( h = t->data[index]; h != NULL; h = h->next ) {
    if ( strcmp(key, h->key) == 0 )
      return -2;
    hh = h;
  }
  
  hh->next = (fh_item *)xmalloc(sizeof(fh_item));
  
  hh->next->key = strdup(key);
  if ( !hh->next->key ) {
    return -1;
  }
  hh->next->data = data;
  hh->next->next = NULL;
  
  return index;
}

int 
fhash_init (fhash *t, int size) 
{
  if ( size < 0 )
    return 0;

  if ( size == 0 )
    size = 250;

  printf("INITIALIZED: %i\n", size);

  if ( t == NULL )
    t = (fhash *)xmalloc(sizeof(fhash));

  t->size = size;
  t->data = (fh_item **) xcalloc(size, sizeof(fh_item));

  int i;
  for ( i = size - 1; i != -1; i-- )
    t->data[i] = 0;  

  return 1;
}





static void 
fhashstack_init (fhashstack *s) 
{
  s->first = NULL;
  s->last  = NULL;
  s->size  = 0;
}

static int 
fhashstack_length (fhashstack *s)
{
  int count;
  if (s->last == NULL)
    return 0;

  fhashstack_item *si = s->first->next;
  for (count = 0; si != s->first; si = si->next) count++;

  return count + 1;
}



static fhashstack_item *
fhashstack_nearest_item (fhashstack *s, unsigned int pos)
{
  unsigned int count;
  fhashstack_item *si;

  if (s->last == NULL)
    return 0;

  if (s->last == s->first)
    return s->first;

  if (pos > s->last->id)
    return s->last;

  //  printf("loc: %i\n", si->id);

  if (pos < s->first->id)
    return s->first;

  if (pos <= (s->last->id / 2))
    {
      for (si = s->first; ; count++)
        si = si->next;
    }
  else
    {
      for (si = s->last->previous; 
           count != pos; 
           count--)
        si = si->previous;
    }

  return si;
}


static fhashstack_item *
fhashstack_peek_item (fhashstack *s, int pos)
{
  int count;
  fhashstack_item *si;

  if (s->last == NULL)
    return 0;

  if (pos > s->size)
    return 0;

  if (pos <= (s->last->id / 2))
    {
      si = s->first;
      for (count = 1; count != pos; count++)
        si = si->next;
    }
  else
    {
      si = s->last;
      for (count = s->size; count != pos; count--)
        si = si->previous;
    }

  return si;
}

static void *
fhashstack_peek (fhashstack *s, unsigned int pos)
{
  fhashstack_item *si;
  if ((si = fhashstack_peek_item(s, pos)) == 0)
    return 0;

  return si->data;
}

static int
fhashstack_insert (fhashstack *s, unsigned int pos, void *item)
{
  fhashstack_item *si;

  fhashstack_item *new_item = (fhashstack_item *) xmalloc (sizeof(fhashstack_item));

  new_item->data = item;
  new_item->id   = pos;
  s->size++;

  /* if nothing exists, allocate our first record */
  if (s->first == NULL)
    {
      s->first = new_item;
      s->last  = new_item;
      
      s->first->next     = new_item;
      s->first->previous = new_item;
    }
  else
    {
      si = fhashstack_nearest_item(s, pos);

      //printf("si: %i\n", si->id);
      if (!si) return;

      if (pos > si->id) /* add entry before */
        {
          /* if this is the first record, we need to set s */
          new_item->previous = si->previous;
          new_item->next     = si;
          si->previous->next = new_item;
          si->previous       = new_item;

          if (si == s->first)
            {
              s->first = new_item;
            }
        }
      else /* add entry after */
        {
          /* if this is the first record, we need to set s */
          new_item->previous = si->previous;
          new_item->next     = si;
          si->previous->next = new_item;
          si->previous       = new_item;

          if (si == s->first)
            {
              //s-> = new_item;
            }
        }
      //puts(si->data);

      //new_item->previous = si->previous;
      //new_item->next     = si;
      //si->previous->next = new_item;
      //si->previous       = new_item;
    }
  
  return 1;
}

static void *
fhashstack_delete (fhashstack *s, unsigned int pos)
{
  fhashstack_item *si;
  if ((si = fhashstack_peek_item(s, pos)) == 0)
    return 0;

  s->size--;

  /* if nothing exists, allocate our first record */
  if (s->last == s->first)
    {
      s->last  = NULL;
      s->first = NULL;
    }
  else
    {
      si->previous->next = si->next;
      si->next->previous = si->previous;
    }

  void *data = si->data;
  free(si);
  return data;
}

#ifdef TEST
#include <stdlib.h>
#include <stdio.h>
int main (int argc, char **argv)
{
  fhashstack t;
  fhashstack_init(&t);
  //  abort();

  int i;
  for (i = 1; i <= 2000000; i++)
    {
      int *x = (int *) malloc (sizeof(int));
      *x = i;
      fhashstack_insert(&t, (int) i/2, "wisdom");
    }

  exit(0);


  if (argc <= 1) abort();

  printf("HASH: %i\n", mk_hash(argv[1]) % 20);
  printf("HASH: %i\n", mk_hash(argv[1]));

  fhash h;
  fhash_init(&h, 20) ;
  
  for (i = 1; i <= 20000; i++)
    {
      int *x = (int *) malloc (sizeof(int));
      *x = i;

      char *k = (char *) malloc (10);
      sprintf(k, "k%i", i);

      fhash_insert(&h, k, x);
    }
  
  fhash_delete(&h, "k200");

  int *ok = fhash_data(&h, "k201");
  printf("value --> %i\n", *ok);


  //printf("hash is: %i\n", mk_hash(argv[1]));
}
#endif
