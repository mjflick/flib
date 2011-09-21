#include <stdlib.h>
#include "alloc.h"
#include "fstack.h"

void 
fstack_init (fstack *s) 
{
  s->first = NULL;
  s->last  = NULL;
  s->size  = 0;
}

int
fstack_push (fstack *s, void *item)
{
  fstack_item *this_item = (fstack_item *) xmalloc (sizeof(fstack_item));

  this_item->data = item;

  s->size++;

  /* if nothing exists, allocate our first record */
  if (s->first == NULL)
    {
      s->first = this_item;
   
      s->last = s->first;
      
      s->first->next     = s->first;
      s->first->previous = s->first;
    }
  else
    {
      fstack_item *old_last = s->last;

      s->last                 = this_item;
      s->last->previous       = old_last;
      s->last->previous->next = s->last;
      s->last->next           = s->first;
      s->first->previous      = s->last;
    }
  
  return 1;
}

int
fstack_unshift (fstack *s, void *item)
{
  fstack_item *this_item = (fstack_item *) xmalloc (sizeof(fstack_item));

  this_item->data = item;

  s->size++;

  /* if nothing exists, allocate our first record */
  if (s->first == NULL)
    {
      s->first = this_item;
   
      s->last = s->first;
      
      s->first->next     = s->first;
      s->first->previous = s->first;
    }
  else
    {
      fstack_item *old_first = s->first;

      s->first                 = this_item;
      s->first->previous       = s->last;
      s->first->next           = old_first;
      s->first->next->previous = s->first;
      s->last->next            = s->first;
    }
  
  return 1;
}

int 
fstack_length (fstack *s)
{
  int count;
  if (s->last == NULL)
    return 0;

  fstack_item *si = s->first->next;
  for (count = 0; si != s->first; si = si->next) count++;

  return count + 1;
}

fstack_item *
fstack_peek_item (fstack *s, int pos)
{
  int count;
  fstack_item *si;

  if (s->last == NULL)
    return 0;

  if (pos > s->size)
    return 0;

  if (pos <= (s->size / 2))
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

void *
fstack_peek (fstack *s, int pos)
{
  fstack_item *si;
  if ((si = fstack_peek_item(s, pos)) == 0)
    return 0;

  return si->data;
}

void *
fstack_pop (fstack *s)
{
  if (s->last == NULL)
    return 0;

  fstack_item *this_item = s->last;

  s->size--;

  if (s->last == s->first)
    {
      s->last  = NULL;
      s->first = NULL;
    }
  else
    {
      s->last->previous->next = s->first;
      s->last = s->last->previous;
      s->first->previous = s->last;
    }

  void *data = this_item->data;
  free(this_item);

  return data;
}

void *
fstack_shift (fstack *s)
{
  if (s->first == NULL)
    return 0;

  fstack_item *this_item = s->first;

  s->size--;

  if (s->last == s->first)
    {
      s->last  = NULL;
      s->first = NULL;
    }
  else
    {
      s->first = s->first->next;
      s->first->previous = s->last;
      s->last->next = s->first;
    }

  void *data = this_item->data;
  free(this_item);

  return data;
}

void *
fstack_shift_peek (fstack *s)
{
  if (s->first == NULL)
    return 0;

  fstack_item *this_item = s->first;

  return this_item->data;
}

void *
fstack_pop_peek (fstack *s)
{
  if (s->last == NULL)
    return 0;

  fstack_item *this_item = s->last;

  return this_item->data;
}

/* fstack_insert will find the pos element and insert before it */

int
fstack_insert (fstack *s, int pos, void *item)
{
  fstack_item *si;
  if (pos == 1)
    return fstack_unshift(s, item);

  if (pos == (s->size + 1))
    return fstack_push(s, item);

  if ((si = fstack_peek_item(s, pos)) == 0)
    return 0;

  fstack_item *new_item = (fstack_item *) xmalloc (sizeof(fstack_item));

  new_item->data = item;
  s->size++;

  /* if nothing exists, allocate our first record */
  if (s->first == NULL)
    {
      s->first = new_item;
      s->last  = s->first;
      
      s->first->next     = s->first;
      s->first->previous = s->first;
    }
  else
    {
      new_item->previous = si->previous;
      new_item->next     = si;
      si->previous->next = new_item;
      si->previous       = new_item;
    }
  
  return 1;
}

void *
fstack_delete (fstack *s, int pos)
{
  fstack_item *si;
  if (pos == 1)
    return fstack_shift(s);

  if (pos == s->size)
    return fstack_pop(s);

  if ((si = fstack_peek_item(s, pos)) == 0)
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
