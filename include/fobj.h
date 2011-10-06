/* define the struct which holds function pointers */

/* provides a simple calling framework
 * objects are called as such $(object, method, params)
 */
#define FOBJ_EXPAND(O, F, ...) O->F(O, ## __VA_ARGS__ )
#define $(...) FOBJ_EXPAND(__VA_ARGS__)

/* new object initialization */
//#define newObj(T, O) T *O = (T *) malloc (sizeof(T)); init ## T(O)
void *flibnewObj(size_t, void (*)( ));

#define newObj(T) (T *) flibnewObj(sizeof(T), init ## T )

