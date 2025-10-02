/**
 * @file regres.h
 *
 * @brief Declaration of linear regression analysis functions
 */

#ifndef REGRES_H
#define REGRES_H


/* Project includes */
#include <dataset.h>


/**
 * @typedef regression_td
 *
 * @brief Structure with all data pertaining regression analysis
 */
typedef struct {
    double a;   /**< Line y-intercept */
    double b;   /**< Line slope */
    double sa;  /**< Statistic error in 'a': 's(a)' */
    double sb;  /**< Statistic error in 'b': 's(b)' */
    double ea;  /**< Propagation error in 'a' */
    double eb;  /**< Propagation error in 'b' */
    double r;   /**< Pearson correlation coefficient (regression: 'r^2') */
} regression_td;


/* Public interface */
/**
 * @brief Compute simple linear regression @e (y = a + b*x) for a data set
 *
 * Performs an ordinary least squares fit on the provided dataset and
 * returns the fitted parameters, their standard errors (approximate),
 * and the Pearson correlation coefficient @e r.
 *
 * @param ds Pointer to the dataset to fit with at least two points
 *
 * @return A regression_td structure with the following fields:
 *           - @e a: y-intercept
 *           - @e b: slope
 *           - @e sa: standard error of the intercept (approximate)
 *           - @e sb: standard error of the slope (approximate)
 *           - @e ea: propagation error of the intercept (approximate)
 *           - @e eb: propagation error of the slope (approximate)
 *           - @e r: Pearson correlation coefficient
 *
 * @note If the dataset has fewer than two points, all fields are zero
 * @note If all @e x values are identical (zero variance in @e x), the
 *       function sets @e b = 0, @e a = mean(y), @e r = 0, and returns
 */
regression_td regres_linear(const dataset_td *ds);


#endif  /* ! REGRES_H */
