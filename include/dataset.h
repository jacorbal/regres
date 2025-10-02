/**
 * @file dataset.h
 *
 * @brief Declaration of dataset management functions
 */

#ifndef DATASET_H
#define DATASET_H


/* System includes */
#include <stddef.h>     /* size_t */


/**
 * @brief Structure representing a data point in the dataset
 */
typedef struct {
    double x;   /**< X abscise of the data point */
    double y;   /**< Y ordinate of the data point */
    double ey;  /**< Error associated with Y coordinate */
} data_point_td;


/**
 * @brief Structure representing a dataseta
 */
typedef struct {
    data_point_td *points;  /**< Pointer to the array of data points */
    size_t capacity;        /**< Maximum points that can be stored */
    size_t size;            /**< Current number of points in the dataset */
    int is_modified;        /**< Flag to tell if dataset has been modified */
} dataset_td;


/* Public interface  */
/**
 * @brief Initialize a dataset structure
 *
 * Set the initial size and capacity of the dataset, allocates memory
 * for the data points, and marks the dataset as unmodified.
 *
 * @param ds Pointer to the dataset structure to be initialized
 */
void dataset_init(dataset_td *ds);

/**
 * @brief Destroy a dataset structure
 *
 * Frees the memory allocated for the data points in the dataset,
 * effectively cleaning up the resources used by the dataset.
 *
 * @param ds Pointer to the dataset structure to be destroyed
 */
void dataset_destroy(dataset_td *ds);

/**
 * @brief Add a new data point to the dataset
 *
 * This function adds a new data point with the specified @e x, @e y,
 * and @e ey values to the dataset.  If the dataset's capacity is
 * reached, it reallocates memory to accommodate more points.
 *
 * @param ds Pointer to the dataset structure where the point will be
 *           added
 *
 * @param x  The @e x value of the new data point
 * @param y  The @e y value of the new data point
 * @param ey The @e y error value of the new data point
 */
void dataset_add(dataset_td *ds, double x, double y, double ey);

/**
 * @brief Apply the logarithm transformation to a specified column
 *
 * Applies the natural logarithm to all data points in the specified
 * column (0 for @e x, 1 for @e y) of the dataset.
 *
 * @param ds  Pointer to the dataset structure.
 * @param col Column index to apply the logarithm transformation
 */
void dataset_log_col(dataset_td *ds, int col);

/**
 * @brief Apply the antilogarithm transformation to a specified column
 *
 * Applies the exponential function to all data points in the specified
 * column (0 for @e x, 1 for @e y) of the dataset.
 *
 * @param ds  Pointer to the dataset structure.
 * @param col Column index to apply the antilogarithm transformation
 */
void dataset_antilog_col(dataset_td *ds, int col);

/**
 * @brief Invert the values in a specified column
 *
 * Inverts the values of all data points in the specified column (0 for
 * @e x, 1 for @e y) of the dataset, provided that the values are not
 * zero.
 *
 * @param ds  Pointer to the dataset structure
 * @param col Column index to invert values
 */
void dataset_inv_col(dataset_td *ds, int col);

/**
 * @brief Multiply the values in a specified column by a factor
 *
 * Multiplies all data points in the specified column (0 for @e x, 1 for
 * @e y) of the dataset by the given factor.
 *
 * @param ds     Pointer to the dataset structure
 * @param col    Column index to multiply values
 * @param factor Factor by which to multiply the values
 */
void dataset_mult_col(dataset_td *ds, int col, double factor);

/**
 * @brief Macro that evaluates to the modified flag of the dataset
 */
#define dataset_is_modified(d) (((d)->is_modified))

/**
 * @brief Macro that evaluates to the size of the dataset
 */
#define dataset_size(d) (((d)->size))

/**
 * @brief Macro that evaluates to the emptiness of the dataset
 */
#define dataset_is_empty(d) (((d)->size) == 0)


#endif  /* ! DATASET_H */
