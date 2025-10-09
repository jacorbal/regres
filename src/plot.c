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


static char *temp_files[PLOT_MAX_TEMP_FILES];   /**< Array to store names
                                                     of temporary files */
static int temp_file_count = 0;                 /**< Currently registered
                                                     temporary files */


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
 *         1 if the maximum limit has been reached,
 *         2 if there was an error duplicating the filename
 */
static int s_tmp_files_register(const char *filename)
{
    if (temp_file_count < PLOT_MAX_TEMP_FILES) {
        temp_files[temp_file_count] = strdup(filename);
        if (temp_files[temp_file_count] == NULL) {
            return 2;
        }
        temp_file_count++;
        return 0;
    }

    return 1;
}


/**
 * @brief Retrieve a non-empty environment variable value
 *
 * Query the environment for a variable and return its value only if it
 * exists and is not an empty string.  If the variable is not set or is
 * empty, the function will return @c NULL.
 *
 * @param name Name of the environment variable to query
 *
 * @return Pointer to the environment string as returned by @a getenv()
 *         if the variable exists and is non-empty, otherwise @c NULL
 *
 * @note The returned pointer is valid until the environment is changed
 *       (e.g., via @a putenv, @a setenv, or @a unsetenv) and should not
 *       be freed
 *
 * @warning This function is not thread-safe with respect to
 *          modifications of the process environment, i.e., concurrent
 *          changes to the environment may make the returned pointer
 *          invalid
 */
static const char *s_getenv_nonempty(const char *name)
{
    const char *v;

    if (name == NULL) {
        return NULL;
    }

    v = getenv(name);
    return (v != NULL && *v != '\0') ? v : NULL;
}


/**
 * @brief Return the first non-empty environment variable value from
 *        a null-terminated list
 *
 * Iterates over a null-terminated array of environment variable names
 * and return the value of the first name that is set in the environment
 * and not empty.
 *
 * @param names Null-terminated array of null-terminated environment
 *              variable name strings
 *
 * @return Pointer to the environment string (as returned by @a getenv)
 *         for the first non-empty variable found, or @c NULL if none
 *         are set/non-empty
 *
 * @note If @p names itself is @c NULL, the return value will be @c NULL
 * @note The returned pointer refers to the process environment storage
 *       and must not be freed
 * @note This function is safe only if the environment is not being
 *       changed concurrently
 *
 * @see @a s_getenv_nonempty()
 */
static const char *s_tmpdir_first_nonempty(const char *names[])
{
    if (names == NULL) {
        return NULL;
    }

    for (size_t i = 0; names[i] != NULL; ++i) {
        const char *v = s_getenv_nonempty(names[i]);
        if (v != NULL) {
            return v;
        }
    }

    return NULL;
}


/* Plot data points and the regression line using 'gnuplot' */
void plot_data(const dataset_td *ds, double a, double b)
{
    FILE *fp;
    FILE *gp;
    int fd;
    char tmpl[256];
    const char *tmp_names[] = { "TMPDIR", "TEMPDIR", "TMP", "TEMP", NULL };
    const char *tmpdir = s_tmpdir_first_nonempty(tmp_names);

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
