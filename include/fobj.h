/* define the struct which holds function pointers */

/* provides a simple calling framework
 * objects are called as such $(object, method, params)
 */
#define FOBJ_EXPAND(O, F, ...) O->F(O, ## __VA_ARGS__ )
#define $(...) FOBJ_EXPAND(__VA_ARGS__)

/* new object initialization */
void *flibnewObj(size_t, void (*)( ));
void *flibcopyObj(size_t, void *s);
#define newObj(T) (T *) flibnewObj(sizeof(T), init ## T )
#define copyObj(T,S) (T *) flibcopyObj(sizeof(T), S)

