#include <stdlib.h>

/* provides a simple calling framework
 * objects are called as such $(object, method, params)
 */
#define FOBJ_EXPAND(O, F, ...) O->F(O, ## __VA_ARGS__ )
#define $(...) FOBJ_EXPAND(__VA_ARGS__)

/* new object initialization */
#define newObj(T, O) T *O = (T *) malloc (sizeof(T)); init ## T(O)

