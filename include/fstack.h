typedef struct fs_item {  
  void *data; 
  struct fs_item *next;
  struct fs_item *previous;  
} fstack_item;

typedef struct {
  fstack_item *first;
  fstack_item *last;
  int size;
} fstack;


void fstack_init (fstack *);
int fstack_push (fstack *, void *);
int fstack_unshift (fstack *, void *);
int fstack_length (fstack *);
fstack_item *fstack_peek_item (fstack *, int);
void *fstack_peek (fstack *, int);
void *fstack_pop (fstack *);
void *fstack_shift (fstack *);
void *fstack_shift_peek (fstack *);
void *fstack_pop_peek (fstack *);
int  fstack_insert (fstack *, int, void *);
void *fstack_delete (fstack *, int);
