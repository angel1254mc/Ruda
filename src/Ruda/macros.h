#include "util/macros.h"
#include "util/u_math.h"
#include "util/rounding.h"
#include "util/compiler.h"

/**
 * Sometimes we treat floats as ints.  On x86 systems, moving a float
 * as an int (thereby using integer registers instead of FP registers) is
 * a performance win.  Typically, this can be done with ordinary casts.
 * But with gcc's -fstrict-aliasing flag (which defaults to on in gcc 3.0)
 * these casts generate warnings.
 * The following union typedef is used to solve that.
 */
typedef union { float f; int i; unsigned u; } fi_type;

static fi_type UINT_AS_UNION(uint u)
{
   fi_type tmp;
   tmp.u = u;
   return tmp;
}

static inline fi_type INT_AS_UNION(int i)
{
   fi_type tmp;
   tmp.i = i;
   return tmp;
}

static inline fi_type FLOAT_AS_UNION(float f)
{
   fi_type tmp;
   tmp.f = f;
   return tmp;
}


/** Copy \p SZ elements into a 4-element vector */
#define COPY_SZ_4V(DST, SZ, SRC)                  \
do {                                              \
   switch (SZ) {                                  \
   case 4: (DST)[3] = (SRC)[3];                   \
           FALLTHROUGH;                           \
   case 3: (DST)[2] = (SRC)[2];                   \
           FALLTHROUGH;                           \
   case 2: (DST)[1] = (SRC)[1];                   \
           FALLTHROUGH;                           \
   case 1: (DST)[0] = (SRC)[0];                   \
   }                                              \
} while(0)
