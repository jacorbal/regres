/**
 * @file plot.h
 *
 * @brief Declaration of plot functions
 */

#ifndef PLOT_H
#define PLOT_H


/* Project includes */
#include <dataset.h>


/**
 * @brief Plot data points and the regression line using @c gnuplot
 *
 * Creates a temporary file with the dataset points, launches @c gnuplot
 * and instructs it to plot the points and the regression line
 * @e (y = a + b*x).
 *
 * @param ds Pointer to the dataset to plot (must not be @c NULL)
 * @param a  Intercept of the regression line @e (y = a + b*x)
 * @param b  Slope of the regression line @e (y = a + b*x)
 *
 * @note A temporary file is needed in case data is not saved
 * @note The temporary data file is intentionally not unlinked
 *       immediately because it will affect the data being read by
 *       @c gnuplot
 * @note If temporary-file creation or @c gnuplot invocation fails, the
 *       function returns without reporting an error.
 */
void plot_data(const dataset_td *ds, double a, double b);


#endif  /* ! PLOT_H  */
