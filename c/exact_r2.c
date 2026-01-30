#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "common.h"

double exact_r2(const double * restrict data, size_t n, dimension_t dim,
               const double * restrict ref)
{
    if (unlikely(n == 0)) return 0.0;
    ASSUME(dim == 2);

    return 1.0;
}
