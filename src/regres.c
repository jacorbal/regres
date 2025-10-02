/**
 * @file regres.c
 *
 * @brief Implementation of linear regression analysis functions
 */

/* System includes */
#include <math.h>       /* sqrt */

/* Local includes */
#include <regres.h>


/* Compute simple linear regression for a data set (y = a + b*x) */
regression_td regres_linear(const dataset_td *ds)
{
    regression_td reg = {0};
    size_t n = ds->size;

    if (n < 2) {
        return reg;
    }

    /* Accumulators for weighted or unweighted normal equations */
    double S = 0.0, Sx = 0.0, Sy = 0.0, Sxx = 0.0, Sxy = 0.0;
    int use_weights = 0;

    /* Check if 'ey' weights are valid */
    for (size_t i = 0; i < n; ++i) {
        if (ds->points[i].ey > 0.0) {
             use_weights = 1;
             break;
        }
    }

    if (use_weights) {
        /* Weighted least squares: weight = 1/ey^2 */
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double ey = ds->points[i].ey;
            double w = (ey > 0.0) ? 1.0 / (ey * ey) : 0.0;
            S   += w;
            Sx  += w * x;
            Sy  += w * y;
            Sxx += w * x * x;
            Sxy += w * x * y;
        }
    } else {
        /* Ordinary least squares (equal weights) */
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double w = 1.0;
            S   += w;
            Sx  += w * x;
            Sy  += w * y;
            Sxx += w * x * x;
            Sxy += w * x * y;
        }
    }

    double delta = S * Sxx - Sx * Sx;
    double a = 0.0, b = 0.0, sa = 0.0, sb = 0.0;
    double ea = 0.0, eb = 0.0;
    double r = 0.0;

    /* Degenerate case */
    if (delta == 0.0) {
        reg.a = 0.0;
        reg.b = 0.0;
        reg.sa = reg.sb = 0.0;
        reg.ea = reg.eb = 0.0;
        reg.r = 0.0;

        return reg;
    }

    /* Best-fit parameters */
    b = (S * Sxy - Sx * Sy) / delta;
    a = (Sxx * Sy - Sx * Sxy) / delta;

    /* Residual sums to estimate variance */
    double chisq = 0.0; /* Used for weighted case */
    double ssr = 0.0;   /* Unweighted sum of squared residuals */
    if (use_weights) {
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double ey = ds->points[i].ey;
            double fit = a + b * x;
            double resid = y - fit;
            double w = (ey > 0.0) ? 1.0 / (ey * ey) : 0.0;

            chisq += w * resid * resid;
            ssr += resid * resid;   /* Keep unweighted SSR if needed */
        }

        /* For weighted fits, parameter covariance matrix is
         * inverse(normal matrix):
         *   - cov(a,a) = Sxx / delta
         *   - cov(b,b) =   S / delta
         *   - cov(a,b) = -Sx / delta
         *
         * If provided ey are true measurement errors, the variances are
         * these diagonal elements
         *
         * If 'ey' are only relative and we want to scale by the reduced
         * chi2, multiply by s2 = chisq/(n-2) */
        double s2 = (n > 2) ? (chisq / (double)(n - 2)) : 1.0;
        /* If ey are true known measurement errors, don't scale
         * (i.e. s2 ~= 1).  Scale by s2 (common practice) */
        sb = sqrt(s2 * (S / delta));
        sa = sqrt(s2 * (Sxx / delta));

        /* Propagation errors ea, eb using your linear propagation formulas:
         *   - ea = sum_i | (n*x_i - S) / den | * ey_i
         *   - eb = sum_i | (Sx2 - x_i*S) / den | * ey_i
         * where den = n*sx2 - sx*sx  (here using weighted equivalents)
         * For weighted case we use S, Sx, Sxx and den = delta */

        double den = delta;
        double sum_a = 0.0, sum_b = 0.0;
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double ey = ds->points[i].ey;
            double coef_a = fabs((S * x - Sx) / den);   /* ~(n*x-sx)/den */
            double coef_b = fabs((Sxx - x * Sx) / den); /* ~(sx2-x*sx)/den */
            if (ey < 0.0) ey = 0.0;
            sum_a += coef_a * ey;
            sum_b += coef_b * ey;
        }
        ea = sum_a;
        eb = sum_b;
    } else {
        /* Unweighted case */
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double fit = a + b * x;
            double resid = y - fit;

            ssr += resid * resid;
        }
        double s2 = (n > 2) ? (ssr / (double)(n - 2)) : 0.0;

        /* Useful combination: Sxx - Sx^2 / S = sum (x - x_mean)^2
         * (for equal weights) */
        double denom = Sxx - (Sx * Sx) / S;
        if (denom <= 0.0) {
            sb = 0.0;
            sa = 0.0;
        } else {
            sb = sqrt(s2 / denom);
            sa = sqrt(s2 * Sxx / (S * denom));

            /* For propagation errors with unweighted data we need
             * per-point 'ey'.  If caller provided ey==0 for all, treat
             * measurement errors as sqrt(s2) (same uncertainty for all
             * points).  Then apply your linear propagation discrete
             * sums with den = n*sx2 - sx*sx. */
            double den = S * Sxx - Sx * Sx; /* Equals delta */
            double sum_a = 0.0, sum_b = 0.0;
            double ey_effective = sqrt(s2); /* Same 'ey' for all points */
            for (size_t i = 0; i < n; ++i) {
                double x = ds->points[i].x;
                double coef_a = fabs((S * x - Sx) / den);
                double coef_b = fabs((Sxx - x * Sx) / den);
                sum_a += coef_a * ey_effective;
                sum_b += coef_b * ey_effective;
            }
            ea = sum_a;
            eb = sum_b;
        }
    }

    /* Pearson correlation coefficient r:
     *   - For unweighted data use standard definition.
     *   - For weighted data use weighted covariance/variances.
     */
    if (use_weights) {
        /* Compute weighted means */
        double xw_mean = Sx / S;
        double yw_mean = Sy / S;
        double cov_xy = 0.0, var_x = 0.0, var_y = 0.0;
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double ey = ds->points[i].ey;
            double w = (ey > 0.0) ? 1.0 / (ey * ey) : 0.0;
            double dx = x - xw_mean;
            double dy = y - yw_mean;

            cov_xy += w * dx * dy;
            var_x += w * dx * dx;
            var_y += w * dy * dy;
        }
        if (var_x > 0.0 && var_y > 0.0) {
            r = cov_xy / sqrt(var_x * var_y);
        } else {
            r = 0.0;
        }
    } else {
        /* Unweighted Pearson r */
        double xmean = Sx / S;
        double ymean = Sy / S;
        double num = 0.0, denx = 0.0, deny = 0.0;
        for (size_t i = 0; i < n; ++i) {
            double x = ds->points[i].x;
            double y = ds->points[i].y;
            double dx = x - xmean;
            double dy = y - ymean;

            num += dx * dy;
            denx += dx * dx;
            deny += dy * dy;
        }
        if (denx > 0.0 && deny > 0.0) {
            r = num / sqrt(denx * deny);
        } else {
            r = 0.0;
        }
    }

    /* Populate the structure */
    reg.a = a;
    reg.b = b;
    reg.sa = sa;
    reg.sb = sb;
    reg.ea = ea;
    reg.eb = eb;
    reg.r = r;

    return reg;
}
