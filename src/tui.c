/**
 * @file tui.c
 *
 * @brief Implementation of TUI routines
 */

#define _POSIX_C_SOURCE  200809L

/* System includes */
#include <stdlib.h>     /* free */

/* Library includes */
#include <menu.h>
#include <ncurses.h>

/* Project includes */
#include <dataset.h>
#include <fileio.h>
#include <global.h>
#include <regres.h>

/* Local includes */
#include <tui.h>
#include <tui/menu.h>
#include <tui/views.h>


static int is_running = 0;  /**< Set the main loop to run if 1 */


/* Start tht TUI */
int tui_start(void)
{
    initscr();

    /* If terminal is too small, exit */
    if (LINES < MIN_LINES || COLS < MIN_COLS) {
        clear();
        mvprintw(LINES / 2, (COLS - 40) / 2, "Terminal size too small!");
        mvprintw(LINES / 2 + 1, (COLS - 40) / 2,
                "Minimum size: %d x %d", MIN_COLS, MIN_LINES);

        curs_set(0);    /* Hide the cursor */
        refresh();
        getch();
        endwin();

        return 1;
    }

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    return 0;
}


/* Main event loop for TUI */
int tui_loop(void)
{
    dataset_td dataset;
    char *cur_filename = NULL;

    dataset_init(&dataset);

    is_running = 1;
    while (is_running) {
        WINDOW *menu_win = NULL;
        WINDOW *menu_sub = NULL;
        ITEM **items = NULL;
        MENU *menu = NULL;
        int items_count = 0;
        int height = TUI_MENU_MAX + 2;

        clear();
        curs_set(0);
        tui_view_print_title(cur_filename,
                dataset_is_modified(&dataset), dataset_is_empty(&dataset));
        refresh();

        menu = tui_menu_create(&menu_win, &menu_sub, &items,
                &items_count, &dataset, cur_filename);
        if (menu == NULL) {
            if (items) {
                for (int i = 0; i < items_count; ++i) {
                    if (items[i]) {
                        free_item(items[i]);
                    }
                }
                free(items);
                items = NULL;
            }
            if (menu_sub) {
                delwin(menu_sub);
                menu_sub = NULL;
            }
            if (menu_win) {
                delwin(menu_win);
                menu_win = NULL;
            }
            continue;
        }

        /* Navigate and get selected index */
        int index = tui_menu_navigate_and_get_index(menu, menu_win);

        /* Execute chosen action (may set 'is_running' to 0) */
        tui_menu_execute_choice(index, &dataset, &cur_filename,
                &is_running);

        /* Centralized cleanup */
        tui_menu_destroy(menu, menu_win, menu_sub, items, items_count);

        menu = NULL;
        menu_win = NULL;
        menu_sub = NULL;
        items = NULL;
    }

    if (cur_filename) {
        free(cur_filename);
        cur_filename = NULL;
    }

    dataset_destroy(&dataset);
    return 0;
}


/* Finishes the TUI */
void tui_end(void)
{
    curs_set(1);    /* Restore cursor */
    endwin();
}
