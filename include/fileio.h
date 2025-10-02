/**
 * @file fileio.h
 *
 * @brief Declaration of file manipulation (load/save) functions
 */

#ifndef FILEIO_H
#define FILEIO_H


/* Project includes */
#include <dataset.h>


/* Public interface */
/**
 * @brief Load data points from a text file into a dataset
 *
 * Reads whitespace-separated columns @e (x, y, [ey]) from the
 * specified file and appends them to the provided dataset.  Lines with
 * fewer than two numeric values are ignored.  If the optional error
 * column (@e ey) is not present on a line, @e ey is set to 0 for that
 * point.
 *
 * @param filename Path to the input text file
 * @param ds       Pointer to the dataset to populate
 *
 * @return 1 on success (file opened and read)
 *         0 on failure (could not open file)
 *
 * @note The function reinitializes the dataset before loading (previous
 *       contents are destroyed)
 * @note On successful load the dataset's @e is_modified flag is cleared
 */
int fileio_load(const char *filename, dataset_td *ds);

/**
 * @brief Save dataset points to a text file
 *
 * Writes each data point in the dataset to the specified file as three
 * floating-point columns: @e (x, y, ey).  After a successful save
 * the dataset's @e is_modified flag is cleared.
 *
 * @param filename Path to the output text file
 * @param ds       Pointer to the dataset to save
 *
 * @return 1 on success (file opened and written),
 *         0 on failure (could not open file)
 */
int fileio_save(const char *filename, dataset_td *ds);


#endif  /* ! FILEIO_H */
