/**
 * @file fileio.c
 *
 * @brief Implementation of file manipulation (load/save) functions
 */

/* System includes */
#include <stdio.h>      /* fopen, fgets, sscanf, size_t */

/* Project includes */
#include <dataset.h>

/* Local includes */
#include <fileio.h>


/* Load data points from a text file into a dataset */
int fileio_load(const char *filename, dataset_td *ds)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return 0;
    }

    dataset_destroy(ds);
    dataset_init(ds);

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        double x, y, ey=0;
        int n = sscanf(line, "%lf %lf %lf", &x, &y, &ey);

        if (n < 2) {
            continue;
        }
        if (n == 2) {
            ey = 0;
        }
        dataset_add(ds, x, y, ey);
    }

    fclose(fp);
    ds->is_modified = 0;

    return 1;
}


/* Save dataset points to a text file */
int fileio_save(const char *filename, dataset_td *ds)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        return 0;
    }

    for (size_t i=0; i < ds->size; ++i) {
        fprintf(fp, "%f %f %f\n",
                ds->points[i].x, ds->points[i].y, ds->points[i].ey);
    }

    fclose(fp);
    ds->is_modified = 0;

    return 1;
}
