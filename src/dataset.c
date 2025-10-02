/**
 * @file dataset.c
 *
 * @brief Implementation of dataset management functions
 */

/* System includes */
#include <math.h>       /* log, exp */
#include <stdlib.h>     /* free, malloc */

/* Local includes */
#include <dataset.h>


/* Initialize a dataset structure */
void dataset_init(dataset_td *ds)
{
    ds->size = 0;
    ds->capacity = 20;
    ds->points = malloc(ds->capacity * sizeof(data_point_td));
    ds->is_modified = 0;
}


/* Destroy a dataset structure */
void dataset_destroy(dataset_td *ds)
{
    free(ds->points);
}


/* Add a new data point to the dataset */
void dataset_add(dataset_td *ds, double x, double y, double ey)
{
    if (ds->size == ds->capacity) {
        ds->capacity *= 2;
        ds->points =
            realloc(ds->points, ds->capacity * sizeof(data_point_td));
    }

    ds->points[ds->size].x = x;
    ds->points[ds->size].y = y;
    ds->points[ds->size].ey = ey;
    ds->size++;
    ds->is_modified = 1;
}


/* Apply the logarithm transformation to a specified column */
void dataset_log_col(dataset_td *ds, int col)
{
    for (size_t i = 0; i < ds->size; ++i) {
        if (col == 0) {
            ds->points[i].x = log(ds->points[i].x);
        } else if (col == 1) {
            ds->points[i].y = log(ds->points[i].y);
        }
    }
}


/* Apply the antilogarithm transformation to a specified column */
void dataset_antilog_col(dataset_td *ds, int col)
{
    for (size_t i = 0; i < ds->size; ++i) {
        if (col == 0) {
            ds->points[i].x = exp(ds->points[i].x);
        } else if (col == 1) {
            ds->points[i].y = exp(ds->points[i].y);
        }
    }
}


/* Invert the values in a specified column */
void dataset_inv_col(dataset_td *ds, int col)
{
    for (size_t i = 0; i < ds->size; ++i) {
        if (col == 0 && ds->points[i].x != 0) {
            ds->points[i].x = 1.0 / ds->points[i].x;
        } else if (col == 1 && ds->points[i].y != 0) {
            ds->points[i].y = 1.0 / ds->points[i].y;
        }
    }
}


/* Multiply the values in a specified column by a factor */
void dataset_mult_col(dataset_td *ds, int col, double factor)
{
    for (size_t i = 0; i < ds->size; ++i) {
        if (col == 0) {
            ds->points[i].x *= factor;
        } else if (col == 1) {
            ds->points[i].y *= factor;
        }
    }
}
