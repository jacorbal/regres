/**
 * @file tui/views.c
 *
 * @brief Implementation of view windows
 */

/* System includes */
#include <math.h>       /* sqrt */
#include <string.h>     /* strlen, strncat, size_t */

/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <dataset.h>
#include <global.h>
#include <regres.h>
#include <stats.h>

/* Local includes */
#include <tui/views.h>


/**
 * @brief Generic viewer for table of labels and values with pagination
 *
 * @param win     Window where to draw the table
 * @param n_lines Number of lines per page
 * @param labels  Array of labels
 * @param values  Array of values of type double
 * @param title   Page title
 */
static void s_gui_view_table(WINDOW *win, size_t n_lines,
        const char *labels[], double values[], const char *title)
{
    /* Pagination setup */
    int inner_h = getmaxy(win) - 4; /* Leave room for title and footer */
    if (inner_h <= 0) {
        inner_h = 1;
    }
    size_t max_rows = (size_t) inner_h;
    size_t total = n_lines;
    size_t pages = (total + max_rows - 1) / max_rows;
    size_t page = 0;

    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "%s (Page %zu/%zu)", title, page + 1, pages);

        size_t start = page * max_rows;
        size_t end = (start + max_rows > total) ? total : start + max_rows;
        for (size_t i = start; i < end; ++i) {
            /* Left label & right value, aligned in a fixed width field
             * reserve (2..(width-2)) inside box */
            int label_col = 2;
            int value_width = 18;
            int value_col = 25;
            if (value_col <= label_col + 1) {
                value_col = label_col + 12;
            }
            mvwprintw(win, 1 + (int) (i - start) + 1, label_col,
                    "%-20s", labels[i]);
            mvwprintw(win, 1 + (int) (i - start) + 1, value_col,
                    "%*.8f", value_width, values[i]);
        }

        mvwprintw(win, getmaxy(win) - 2, 2, "n: next, p: prev, q: back");
        wrefresh(win);

        int ch = wgetch(win);
        if (ch == 27/*ESC*/ || ch == 'q' || ch == 'Q') {
            break;
        }
        if (ch == 'n' || ch == 'N') {
            if (page + 1 < pages) {
                page++;
            }
        } else if (ch == 'p' || ch == 'P') {
            if (page > 0) {
                page--;
            }
        }
    } /* ! while(1)  */
}


/* Print title header and a mark regarding if data is saved */
void tui_view_print_title(const char *filename,
        int is_modified, int is_empty)
{
    int col;
    const char *name = (filename) ? filename : "";
    size_t name_len = strlen(name);
    char mark[16] = "";
    int total_right_len;

    if (is_modified) {
        strncat(mark, "[modified]", sizeof(mark) - 1);
    }
    if (is_empty) {
        strncat(mark, "[no data]", sizeof(mark) - 1);
    }

    mvprintw(0, 0, "%s %s", REGRES_PROG_NAME, REGRES_PROG_VERSION);

    total_right_len = (int) name_len +
        (mark[0] ? (1 + (int) strlen(mark)) : 0);
    col = COLS - total_right_len;   /* -1 ? */

    if (col < (int) (strlen(REGRES_PROG_NAME) + 1)) {
        int avail = COLS - 1 - (mark[0] ? (1 + (int) strlen(mark)) : 0);
        if (avail > 0) {
            const char *start = name;
            if ((int) name_len > avail) {
                start = name + (name_len - avail);
            }
            mvprintw(0, 1, "%.*s", avail, start);
        }
    } else {
        if (mark[0]) {
            mvprintw(0, col, "%s %s", name, mark);
        } else {
            mvprintw(0, col, "%s", name);
        }
    }
    refresh();
}


/* View for input data from user */
void tui_view_input_data(dataset_td *ds, WINDOW *win)
{
    echo();

    while (1) {
        double x, y, ey = 0;
        int ch;

        werase(win);
        box(win,0,0);

        mvwprintw(win,1,2,"Enter data point (x y [error in y])");
        mvwprintw(win,4,4,"x: ");
        wrefresh(win);

        curs_set(1);
        wscanw(win, "%lf", &x);
        mvwprintw(win,5,4,"y: ");
        wscanw(win, "%lf", &y);
        mvwprintw(win,6,4,"Error in Y (0 if none): ");
        wscanw(win, "%lf", &ey);
        curs_set(0);

        mvwprintw(win,2,2,"Press 'q' to stop, or ENTER to continue");
        dataset_add(ds, x, y, ey);

        ch = wgetch(win);
        if (ch == 27/*ESC*/ || ch == 'q' || ch == 'Q') {
            break;
        }
    }

    noecho();
}


/* View for showing the data tables */
void tui_view_show_data(const dataset_td *ds, WINDOW *win)
{
    size_t max_rows = getmaxy(win) - 4;
    size_t total = ds->size;
    size_t pages = (total + max_rows - 1) / max_rows;
    size_t page=0;

    while (1) {
        int ch;
        size_t start_idx;
        size_t end_idx;

        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Data Table (Page %zu/%zu)", page+1, pages);
        mvwprintw(win, 1, 2,
                "%4s    %-14s %-14s %-14s", "i", "X", "Y", "ErrorY");
        start_idx = page * max_rows;
        end_idx = (start_idx + max_rows > total)
            ? total
            : start_idx + max_rows;

        for (size_t i= start_idx; i < end_idx; ++i) {
            mvwprintw(win, 2 + i - start_idx, 2,
                "%4zu    %-14.8f %-14.8f %-14.8f",
                i + 1, ds->points[i].x, ds->points[i].y, ds->points[i].ey);
        }

        mvwprintw(win, getmaxy(win)-2, 2, "n: next, p: prev, q: back");
        wrefresh(win);

        ch = wgetch(win);
        if (ch == 27/*ESC*/ || ch == 'q' || ch == 'Q') {
            break;
        }

        if (ch == 'n' || ch == 'N') {
            if (page + 1 < pages) {
                page++;
            }
        } else if (ch == 'p' || ch == 'P') {
            if (page > 0) {
                page--;
            }
        }
    }
}


/* View stats */
void tui_view_stats(const stats_td stats, WINDOW *win)
{
    const char *labels[] = {
        "Mean X:", "Mean Y:", "Sum(x):", "Sum(y):",
        "Sum(x^2):", "Sum(y^2):", "(Sum(x))^2:", "(Sum(y))^2:",
        "Sum(x*y):", "Sum((x-xmn)^2):", "Sum((y-ymn)^2):",
        "s_n(x):", "s_n(y):", "(s_n(x))^2:", "(s_n(y))^2:",
        "s_n-1(x):", "s_n-1(y):", "(s_n-1(x))^2:", "(s_n-1(y))^2:",
        "s_xmn:", "s_ymn:"
    };
    double values[] = {
        stats.x_mean, stats.y_mean, stats.sum_x, stats.sum_y,
        stats.sum_x2, stats.sum_y2,
        stats.sum_x * stats.sum_x, stats.sum_y * stats.sum_y,
        stats.sum_xy, stats.ssx, stats.ssy,
        stats.snx, stats.sny,
        stats.snx * stats.snx, stats.sny * stats.sny,
        stats.snxn1, stats.snyn1,
        stats.snxn1 * stats.snxn1, stats.snyn1 * stats.snyn1,
        sqrt(stats.ssx) / sqrt(stats.n),
        sqrt(stats.ssy) / sqrt(stats.n)
    };
    size_t n_lines = sizeof(values) / sizeof(values[0]);

    s_gui_view_table(win, n_lines, labels, values, "Statistics");
}


/* View regression analysis */
void tui_view_regression(const regression_td reg, WINDOW *win)
{
    const char *labels[] = {
        "a [intercept]", "b [slope]",
        "s(a)", "s(b)", "e(a)", "e(b)",
        "r", "r^2"
    };
    double values[] = {
        reg.a, reg.b, reg.sa, reg.sb, reg.ea, reg.eb,
        reg.r, reg.r * reg.r
    };
    size_t n_lines = sizeof(values) / sizeof(values[0]);

    s_gui_view_table(win, n_lines, labels, values,
            "Linear regression (y=a+bx)");
}
