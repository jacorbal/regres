/**
 * @file stats.c
 *
 * @brief Implementation of statistical analysis functions
 */

/* System includes */
#include <stddef.h>     /* size_t */

/* Local includes */
#include <stats.h>


/* Compute statistics and populate the structure */
stats_td stats_compute(const dataset_td *ds)
{
    stats_td stats;
    size_t n = ds->size;
    double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_y2 = 0, sum_xy = 0;

    for (size_t i = 0; i < n; ++i) {
        double x = ds->points[i].x;
        double y = ds->points[i].y;
        sum_x += x;
        sum_y += y;
        sum_x2 += x * x;
        sum_y2 += y * y;
        sum_xy += x * y;
    }

    double xmn = sum_x / n;
    double ymn = sum_y / n;
    double ssx = 0., ssy = 0.;
    double  snx = 0., sny = 0., snxn1 = 0., snyn1 = 0.;
    for (size_t i = 0; i < n; ++i) {
        ssx += (ds->points[i].x - xmn) * (ds->points[i].x - xmn);
        ssy += (ds->points[i].y - ymn) * (ds->points[i].y - ymn);
    }
    snx = ssx / n;
    sny = ssy / n;
    snxn1 = ssx / (n - 1);
    snyn1 = ssy / (n - 1);

    /* Populate the structure */
    stats.n = n;
    stats.x_mean = xmn;
    stats.y_mean = ymn;
    stats.sum_x = sum_x;
    stats.sum_y = sum_y;
    stats.sum_x2 = sum_x2;
    stats.sum_y2 = sum_y2;
    stats.sum_xy = sum_xy;
    stats.ssx = ssx;
    stats.ssy = ssy;
    stats.snx = snx;
    stats.sny = sny;
    stats.snxn1 = snxn1;
    stats.snyn1 = snyn1;

    return stats;
}
