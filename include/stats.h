/**
 * @file stats.h
 *
 * @brief Declaration of statistical analysis functions
 */

#ifndef STATS_H
#define STATS_H


/* System includes */
#include <stddef.h>     /* size_t */

/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <dataset.h>


/**
 * @typedef stats_td
 */
typedef struct {
    size_t n;
    double x_mean;  /**< Mean X: xm */
    double y_mean;  /**< Mean Y: ym */
    double sum_x;   /**< Sum(x) */
    double sum_y;   /**< Sum(y) */
    double sum_x2;  /**< (Sum(x))^2 */
    double sum_y2;  /**< (Sum(y))^2 */
    double sum_xy;  /**< Sum(x*y) */
    double ssx;     /**< Sum((x-xm)^2) */
    double ssy;     /**< Sum((y-ym)^2 */
    double snx;     /**< s_n(x) */
    double sny;     /**< s_n(y) */
    double snxn1;   /**< s_n-1(x) */
    double snyn1;   /**< s_n-1(y) */
} stats_td;


/* Public interface */
/**
 * @brief Compute statistics from the given dataset
 * 
 * Calculates various statistical measures, including the mean, sum, and
 * variance of the @e x and @e y values in the dataset.  It populates
 * a @e stats_td structure with the computed values.
 * 
 * @param ds Pointer to the dataset structure containing the data points
 *
 * @return Structure containing the computed statistics, including the
 *         number of points, means, sums, and variances
 */
stats_td stats_compute(const dataset_td *ds);


#endif  /* ! STATS_H */
