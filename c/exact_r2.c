#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "common.h"
#include "sort.h"

double _utility(double y1, double y2, double y2p) {
    double w, wp;

    if (y1 == DBL_MAX || y1 == 0) {
        return 0;
    }

    if (y2 == DBL_MAX) {
        w = 1;
    } else {
        w = y2 / (y1 + y2);
    }

    if (y2p == DBL_MAX) {
        wp = 1;
    } else {
        wp = y2p / (y1 + y2p);
    }

    return 0.5 * y1 * (wp * wp - w * w);
}

double exact_r2(const double * restrict data, size_t n, dimension_t dim,
               const double * restrict ref)
{
    if (unlikely(n == 0)) return -1;

    ASSUME(dim == 2);

    // p is sorted by f1 (primarily), then f2 (secondarily)
    const double **p = generate_sorted_doublep_2d(data, &n, DBL_MAX);

    if (unlikely(!p)) return -1;

    size_t j = 0;

    // // skip points not dominated by "ideal" ref point
    // while (p[j][0] < ref[0]) {
    //     j++;
    // }

    double exact_r2 = 0;
    
    // first element
    exact_r2 += _utility(p[j][0] - ref[0], p[j][1] - ref[1], DBL_MAX);
    printf("y2 segment: %f, %f, MAX: %f\n", p[j][0] - ref[0], p[j][1] - ref[1], _utility(p[j][0] - ref[0], p[j][1] - ref[1], DBL_MAX));
    double prev_y1 = p[j][0] - ref[0];
    double prev_y2 = p[j][1] - ref[1];

    double y1, y2;

    while(j < n - 1) {
        j++;
        y1 = p[j][0] - ref[0];
        y2 = p[j][1] - ref[1];

        // printf("%f\n", y1);
        // printf("%f\n", y2);

        if (y2 < prev_y2) {
            // pj not dominated by previous non-dominated pj
            exact_r2 += _utility(prev_y2, prev_y1, y1);
            exact_r2 += _utility(y1, y2, prev_y2);

            printf("y1 segment: %f, %f, %f: %f\n", prev_y2, prev_y1, y1, _utility(prev_y2, prev_y1, y1));
            printf("y2 segment: %f, %f, %f: %f\n", y1, y2, prev_y2, _utility(y1, y2, prev_y2));

            prev_y1 = y1;
            prev_y2 = y2;
        }
    }

    // last element
    exact_r2 += _utility(prev_y2, prev_y1, DBL_MAX);
    printf("y1 segment: %f, %f, MAX: %f\n", prev_y2, prev_y1, _utility(prev_y2, prev_y1, DBL_MAX));

    free(p);
    return exact_r2;
}
