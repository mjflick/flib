#include <stdlib.h>
#include <string.h>

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

