/**
 * @file plot.c
 *
 * @brief Implementation of plot functions
 */

#define _POSIX_C_SOURCE 200809L /* popen, pclose, waitpid */


/* System includes */
#include <stdio.h>      /* fdopen, fopen, popen, pclose, size_t */
#include <stdlib.h>     /* getenv, mkstemp, atexit */
#include <string.h>     /* strdup */
#include <unistd.h>     /* close, unlink */

/* Project includes */
#include <dataset.h>

/* Local includes */
#include <plot.h>


static char *temp_files[PLOT_MAX_TEMP_FILES];   /**< Array to store
                                                     names of temp. files */
static int temp_file_count = 0;                 /**< Currently
                                                     registered temp. files */


/**
 * @brief Delete all registered temporary files
 *
 * Iterates through the array of temporary file names, unlinking
 * (deleting) each file and freeing the associated memory.
 *
 * @note Intended to be called at program exit to ensure 
 *       all temporary files are cleaned up
 * @note It should handle situations where files might not exist or
 *       where memory allocation may fail
 */
static void s_tmp_files_delete(void)
{
    for (int i = 0; i < temp_file_count; ++i) {
        if (temp_files[i] != NULL) {
            unlink(temp_files[i]);
            free(temp_files[i]);
            temp_files[i] = NULL;
        }
    }
}


/**
 * @brief Register a temporary file name
 *
 * Attempts to add a new temporary file name to the @e temp_files
 * array.  If the maximum limit defined by @e PLOT_MAX_TEMP_FILES has
 * been reached, it will return an error code.
 *
 * @param filename Name of the temporary file to register
 *
 * @return 0 if the file was successfully registered,
 *         1 if there was an error duplicating the filename,
 *         2 if the maximum limit has been reached
 */
static int s_tmp_files_register(const char *filename)
{
    if (temp_file_count < PLOT_MAX_TEMP_FILES) {
        temp_files[temp_file_count] = strdup(filename);
        if (temp_files[temp_file_count] == NULL) {
            return 1;
        }
        temp_file_count++;
        return 0;
    }

    return 2;
}



/* Plot data points and the regression line using 'gnuplot' */
void plot_data(const dataset_td *ds, double a, double b)
{
    FILE *fp;
    FILE *gp;
    int fd;
    char tmpl[256];
    const char *tmpdir;

    /* Create temporary file and copy the data points to it */
    if ((tmpdir = getenv("TMPDIR")) == NULL &&
        (tmpdir = getenv("TEMPDIR")) == NULL &&
        (tmpdir = getenv("TMP")) == NULL) {
        tmpdir = "/tmp";
    }
    snprintf(tmpl, sizeof(tmpl), "%s/regres_dat_XXXXXX", tmpdir);
    atexit(s_tmp_files_delete);

    if ((fd = mkstemp(tmpl)) == -1) {
        return;
    }

    if ((fp = fdopen(fd, "w")) == NULL ){
        close(fd);
        unlink(tmpl);
        return;
    }

    for (size_t i = 0; i < ds->size; ++i) {
        fprintf(fp, "%f %f\n", ds->points[i].x, ds->points[i].y);
    }
    fflush(fp);
    fclose(fp);
    s_tmp_files_register(tmpl);

    /* Execute 'gnuplot' */
    if ((gp = popen("gnuplot -p 2>/dev/null", "w")) == NULL) {
        unlink(tmpl);
        return;
    }

    fprintf(gp, "set grid\n");
    fprintf(gp, "set title 'Data plot with regression'\n");
    fprintf(gp, "plot '%s' title 'Data points' with"
                " points pointtype 2 pointsize 1, %f + %f*x"
                " with lines linewidth 1 lc rgb 'red'"
                " title 'Regression'\n",
            tmpl, a, b);

    fflush(gp);
    pclose(gp);

    return;
}
