/**
 * @file tui/views.h
 *
 * @brief Declaration of view windows
 */

#ifndef TUI_VIEWS_H
#define TUI_VIEWS_H


/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <dataset.h>
#include <stats.h>


/* Public interface */
/**
 * @brief Print title header and a mark regarding if data is saved
 *
 * @param filename    File name to show, if not @c NULL
 * @param is_modified If non-zero, draws a mark on the top right corner
 * @param is_empty    If non-zero, draws a zero on the top right corner
 */
void tui_view_print_title(const char *filename,
        int is_modified, int is_empty);

/**
 * @brief View for input data from user
 *
 * @param ds  Data set where to store the data
 * @param win Window where to print
 */
void tui_view_input_data(dataset_td *ds, WINDOW *win);

/**
 * @brief View for showing data tables
 *
 * @param ds  Data set where to read the data
 * @param win Window where to print
 */
void tui_view_show_data(const dataset_td *ds, WINDOW *win);

/**
 * @brief Statistics view
 *
 * @param stats Statistic structure populated with all values
 * @param win   Window where to print
 */
void tui_view_stats(const stats_td stats, WINDOW *win);

/**
 * @brief Regression analysis view
 *
 * @param stats Regression structure populated with all values
 * @param win   Window where to print
 */
void tui_view_regression(const regression_td regression, WINDOW *win);


#endif  /* ! TUI_VIEWS_H */
