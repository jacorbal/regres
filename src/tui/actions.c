/**
 * @file tui/actions.c
 *
 * @brief Actions routines implementation
 */

#define _POSIX_C_SOURCE 200809L /* strdup */


/* System includes */
#include <string.h>     /* strdup */
#include <stdlib.h>     /* free */
#include <unistd.h>     /* getcwd */

/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <dataset.h>
#include <fileio.h>
#include <global.h>
#include <plot.h>
#include <regres.h>
#include <stats.h>

/* Local includes */
#include <tui/actions.h>
#include <tui/dialogs.h>
#include <tui/views.h>


/**
 * @brief Return the current working directory as a persistent string
 *
 * Obtains the process current working directory and returns a pointer
 * to an internal static buffer that holds the null-terminated
 * pathname.
 *
 * @return Pointer to a statically allocated null-terminated string
 *         containing the current working directory on success, or
 *         @c NULL on failure
 *
 * @note Returned pointer remains valid until the next call to this
 *       function
 */
static const char *s_get_cwd_msg(void)
{
    static char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        return NULL;
    }

    return cwd;
}


/* Handle input action for the dataset */
void tui_action_input(dataset_td *dataset)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);

    keypad(win, TRUE);
    tui_view_input_data(dataset, win);
    delwin(win);
}


/* Handle load action for the dataset */
void tui_action_load(dataset_td *dataset, char **cur_filename)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);

    keypad(win, TRUE);
    echo();
    mvwprintw(win, 1, 2, "Current directory: '%s'", s_get_cwd_msg());
    /*
    mvwprintw(win, 3, 2, "Current file: '%s'",
          (*cur_filename) ? *cur_filename : "(none)");
    */
    mvwprintw(win, 2, 2, "Enter filename to load: ");
    wrefresh(win);

    char filename[256];
    curs_set(1);
    wgetnstr(win, filename, sizeof(filename) - 1);
    curs_set(0);
    noecho();

    if (fileio_load(filename, dataset) != 0) {
        mvwprintw(win, 4, 2, "Failed to load");
    } else {
        mvwprintw(win, 4, 2, "Data loaded from '%s'", filename);
        free(*cur_filename);
        *cur_filename = strdup(filename);
        if (!*cur_filename) {
            mvwprintw(win, 5, 2,
                    "Failed to store filename (insufficient memory)");
/*
        } else {
            mvwprintw(win, 5, 2, "Current file: %s", *cur_filename);
*/
        }
    }

    wrefresh(win);
    wgetch(win);
    delwin(win);
}


/* Handle save action for the dataset */
void tui_action_save(dataset_td *dataset, char **cur_filename)
{
    if (cur_filename && cur_filename) {
        /* Prompt if data is modified */
        if (!tui_dialog_confirm_if_modified(dataset->is_modified,
                    "Save changes to current file? (y/N)")) {
            return;
        }

        /* Try to save in current name */
        if (fileio_save(*cur_filename, dataset) != 0) {
            /* Failed to save */
            WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);
            keypad(win, TRUE);
            mvwprintw(win, 2, 2, "Failed to save to '%s'",
                    *cur_filename);
            wrefresh(win);
            wgetch(win);
            delwin(win);
            return;
        }

        /* Success */
        dataset->is_modified = 0;
/*
        WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);
        keypad(win, TRUE);
        mvwprintw(win, 2, 2, "Data saved to '%s'", *cur_filename);
        wrefresh(win);
        wgetch(win);
        delwin(win);
*/
    }
}


/* Handle save as action for the dataset */
void tui_action_saveas(dataset_td *dataset, char **cur_filename)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);

    keypad(win, TRUE);
    echo();
    mvwprintw(win, 1, 2, "Current directory: '%s'", s_get_cwd_msg());
    mvwprintw(win, 2, 2, "Enter new filename to save: ");
    wrefresh(win);

    char filename[256];
    curs_set(1);
    wgetnstr(win, filename, sizeof(filename) - 1);
    curs_set(0);
    noecho();

    if (fileio_save(filename, dataset) != 0) {
        mvwprintw(win, 4, 2, "Failed to save");
    } else {
        mvwprintw(win, 4, 2, "Data saved");
        /* Replace current filename */
        if (cur_filename) {
            free(*cur_filename);
            *cur_filename = strdup(filename);
            if (!*cur_filename) {
                mvwprintw(win, 5, 2,
                        "Failed to store filename (insufficient memory)");
/*
            } else {
                mvwprintw(win, 5, 2, "Current file: '%s'", *cur_filename);
*/
            }
        }
    }

    wrefresh(win);
    wgetch(win);
    delwin(win);
}


/* Show the data in the dataset */
void tui_action_show_data(const dataset_td *dataset)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);

    keypad(win, TRUE);
    tui_view_show_data(dataset, win);
    delwin(win);
}

/* Plot the data from the dataset */
void tui_action_plot(const dataset_td *dataset)
{
    regression_td reg;
    reg = regres_linear(dataset);
    plot_data(dataset, reg.a, reg.b);
}


/* Compute and show statistics for the dataset */
void tui_action_stats(const dataset_td *dataset)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);
    stats_td stats;

    keypad(win, TRUE);
    stats = stats_compute(dataset);
    tui_view_stats(stats, win);
    delwin(win);
}


/*a Show regression analysis for the dataset */
void tui_action_regres(const dataset_td *dataset)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);

    keypad(win, TRUE);
    regression_td reg;
    reg = regres_linear(dataset);
    tui_view_regression(reg, win);

    delwin(win);
}


/* Show information about the program */
void tui_action_about(void)
{
    WINDOW *win = newwin(LINES - 4, COLS - 4, 2, 2);
    keypad(win, TRUE);

    werase(win);
    box(win,0,0);

    mvwprintw(win, 1, 2, "About:");
    mvwprintw(win, 3, 2,
            "This program allows for data input, saving, loading,");
    mvwprintw(win, 4, 2,
            "plotting, and statistical & linear regression analysis.");
    mvwprintw(win, 6, 2,
            "Originally developed at the University of Salamanca in");
    mvwprintw(win, 7, 2,
            "2002, 'Regres' was designed to calculate the results");
    mvwprintw(win, 8, 2,
            "needed for the first year of Licentiate degree in Physics.");
    mvwprintw(win, 9, 2,
            "It has proven to be a valuable tool for students who");
    mvwprintw(win, 10, 2,
            "preferred not to perform calculations by hand, allowing");
    mvwprintw(win, 11, 2,
            "them to focus more on grasping concepts and analyzing");
    mvwprintw(win, 12, 2,
            "results rather than getting bogged down in dull calculus.");
    mvwprintw(win, getmaxy(win) - 6, 2, "%s, version %s (%s)",
            REGRES_PROG_NAME, REGRES_PROG_VERSION, REGRES_PROG_BUILD);
    mvwprintw(win, getmaxy(win) - 5, 2, "Licensed under %s",
            REGRES_LICENSE);
    mvwprintw(win, getmaxy(win) - 4, 2, "%s, %s <%s>",
            REGRES_COPYRIGHT, REGRES_AUTHOR, REGRES_AUTHOR_EMAIL);
    mvwprintw(win, getmaxy(win) - 2, 2, "Press any key...");

    wrefresh(win);
    wgetch(win);
    delwin(win);
}
