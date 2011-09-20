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

