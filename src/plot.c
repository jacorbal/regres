/**
 * @file plot.c
 *
 * @brief Implementation of plot functions
 */

#define _POSIX_C_SOURCE 200809L /* popen, pclose */


/* System includes */
#include <stdio.h>      /* fdopen, fopen, popen, pclose, size_t */
#include <stdlib.h>     /* getenv, mkstemp */
#include <unistd.h>     /* close, unlink */

/* Project includes */
#include <dataset.h>

/* Local includes */
#include <plot.h>


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

    /* NOTE. If unlink now, data file won't exist if 'gnuplot' needs to
     *       update.  Let's hope the system works well and it removes
     *       temporary files automatically after a while ('reboot',
     *       'systemd-tmpfiles-clean', &c.) */
    //unlink(tmpl);
}
