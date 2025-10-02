/**
 * @file tui/dialogs.c
 *
 * @brief Implementation of dialogs routines
 *
 */

/* System includes */
#include <string.h>     /* strlen, size_t */

/* Library includes */
#include <ncurses.h>

/* Local includes */
#include <tui/dialogs.h>


/* Present a message if data size is zero */
int tui_dialog_alert_no_data(size_t data_size, const char *msg)
{
    if (data_size) {
        return 0;
    }

    int h = 5, w = (int) strlen(msg) + 4;
    if (w < 20) {
        w = 20;
    }
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW *win = newwin(h, w, y, x);
    if (win == NULL) {
        /* Fallback: draw on 'stdscr' */
        clear();
        mvprintw(2, 2, "%s", msg);
        refresh();
        getch();
        return 1;
    }

    box(win, 0, 0);
    mvwprintw(win, 2, 2, "%s", msg);
    wrefresh(win);
    wgetch(win);
    delwin(win);

    /* Fefresh 'stdscr' so background is redrawn */
    touchwin(stdscr);
    refresh();

    return 1;
}


/* Modal dialog asking whether to do something when data is not saved */
int tui_dialog_confirm_if_modified(int is_modified, const char *msg)
{
    if (!is_modified) {
        /* Already saved, no need to ask */
        return 1;
    }

    int result = 0;
    int width = strlen(msg) + 4;
    WINDOW *win = newwin(5, width, (LINES - 5) / 2, (COLS - 40) / 2);
    if (win == NULL) {
        /* If window cannot be created, asume exit */
        return 1;
    }

    box(win, 0, 0);
    mvwprintw(win, 2, 2, "%s", msg);
    wrefresh(win);

    int ch = wgetch(win);
    delwin(win);

    if (ch == 'y' || ch == 'Y') {
        result = 1; /* Confirm exit */
    } else {
        result = 0; /* Cancel exit */
    }

    return result;
}
