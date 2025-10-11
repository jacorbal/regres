/**
 * @file tui/menu.c
 *
 * @brief Implementation of menu routines
 */

/* System includes */
#include <stdlib.h>     /* free, malloc */
#include <string.h>     /* strlen, size_t */

/* Library includes */
#include <menu.h>
#include <ncurses.h>

/* Local includes */
#include <tui/actions.h>
#include <tui/dialogs.h>
#include <tui/menu.h>


/**
 * @brief Get the size of the longest string in an array of strings
 *
 * @param strs Array of strings
 *
 * @return Size of the longest string in an array of strings
 */
static size_t s_max_strlen(const char *strs[])
{
    if (strs == NULL) {
        return 0;
    }

    size_t max = 0;

    for (const char **p = strs; *p != NULL; ++p) {
        if (*p == NULL) {
            break;
        }
        size_t len = strlen(*p);
        if (len > max) {
            max = len;
        }
    }

    return max;
}


/* Create a menu for the terminal user interface */
MENU *tui_menu_create(WINDOW **out_menu_win, WINDOW **out_menu_sub,
        ITEM ***out_items, int *out_items_count,
        dataset_td *dataset, const char *cur_filename)
{
    const char *choices[] = {
        "Input new data",
        "Load data from file",
        "Save current data",
        "Save as",
        "Show data table",
        "Plot graph",
        "Statistics",
        "Linear regression",
        "About",
        "Quit",
        NULL
    };
    int height = TUI_MENU_MAX + 2;
    int width = s_max_strlen(choices) + 4;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW *menu_win = newwin(height, width, starty, startx);
    if (!menu_win) {
        return NULL;
    }

    keypad(menu_win, TRUE);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, 2, "Main Menu");
    wrefresh(menu_win);

    ITEM **items = malloc((TUI_MENU_MAX + 1) * sizeof(ITEM*));
    if (items == NULL) {
        delwin(menu_win);
        return NULL;
    }

    int items_created = 0;
    for (int i = 0; i < TUI_MENU_MAX; ++i) {
        items[i] = new_item(choices[i], "");
        if (!items[i]) {
            break;
        }

        if (dataset_is_empty(dataset)) {
            /* Disable menu options that do not work without data */
            if (i == TUI_MENU_SAVE_DATA  || i == TUI_MENU_SAVEAS_DATA ||
                i == TUI_MENU_SHOW_TABLE || i == TUI_MENU_PLOT ||
                i == TUI_MENU_STATISTICS || i == TUI_MENU_REGRESSION) {
                int opts = item_opts(items[i]);
                opts &= ~O_SELECTABLE;
                set_item_opts(items[i], opts);
            }
        } else if (!dataset_is_modified(dataset)) {
            /* Disable menu options that do not work with saved data */
            if (i == TUI_MENU_SAVE_DATA) {
                int opts = item_opts(items[i]);
                opts &= ~O_SELECTABLE;
                set_item_opts(items[i], opts);
            }
        }
        ++items_created;
    }

    if (items_created == 0) {
        free(items);
        delwin(menu_win);
        return NULL;
    }
    items[items_created] = NULL;

    MENU *menu = new_menu((ITEM **) items);
    if (menu == NULL) {
        for (int i = 0; i < items_created; ++i) {
            if (items[i]) {
                free_item(items[i]);
            }
        }
        free(items);
        delwin(menu_win);
        return NULL;
    }

    WINDOW *menu_sub = derwin(menu_win, height - 2, width - 2, 1, 1);
    if (menu_sub == NULL) {
        unpost_menu(menu);
        free_menu(menu);
        for (int i = 0; i < items_created; ++i) {
            if (items[i]) {
                free_item(items[i]);
            }
        }
        free(items);
        delwin(menu_win);
        return NULL;
    }

    int mopts = menu_opts(menu);
    mopts &= ~O_NONCYCLIC;
    set_menu_opts(menu, mopts);
    set_menu_sub(menu, menu_sub);
    post_menu(menu);
    wrefresh(menu_win);

    *out_menu_win = menu_win;
    *out_menu_sub = menu_sub;
    *out_items = items;
    *out_items_count = items_created;

    return menu;
}


/* Cleanup and free resources */
void tui_menu_destroy(MENU *menu, WINDOW *menu_win, WINDOW *menu_sub,
        ITEM **items, int items_count)
{
    if (menu) {
        unpost_menu(menu);
        free_menu(menu);
    }

    if (items) {
        for (int i = 0; i < items_count; ++i) {
            if (items[i]) {
                free_item(items[i]);
                items[i] = NULL;
            }
        }
        free(items);
    }

    if (menu_sub) {
        delwin(menu_sub);
    }

    if (menu_win) {
        delwin(menu_win);
    }
}


/* Navigate and get selected index */
int tui_menu_navigate_and_get_index(MENU *menu, WINDOW *menu_win)
{
    int c;
    while ((c = wgetch(menu_win)) != '\n') {
        switch (c) {
            case KEY_DOWN:
            case 'j':
            case 'J':
                menu_driver(menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
            case 'k':
            case 'K':
                menu_driver(menu, REQ_UP_ITEM);
                break;

            default:
                break;
        }
        wrefresh(menu_win);
    }

    ITEM *cur = current_item(menu);
    return item_index(cur);
}


/* Execute chosen action corresponding to the selected menu item */
void tui_menu_execute_choice(int index, dataset_td *dataset,
        char **cur_filename, int *is_running)
{
    switch (index) {
        case TUI_MENU_INPUT_DATA:
            tui_action_input(dataset);
            break;

        case TUI_MENU_LOAD_DATA:
            if (!tui_dialog_confirm_if_modified(
                        dataset_is_modified(dataset),
                        "Unsaved data! Load another file anyway?"
                        " (y/N)")) {
                break;
            }
            tui_action_load(dataset, cur_filename);
            break;

        case TUI_MENU_SAVE_DATA:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data entered: enter new data or load"
                        " an existing file") != 0){
                break;
            }
            if (tui_dialog_alert_on_condition(
                        dataset_is_modified(dataset),
                        "No changes have been made: no need to"
                        " save file") != 0){
                break;
            }
            tui_action_save(dataset, cur_filename);
            break;

        case TUI_MENU_SAVEAS_DATA:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data entered: enter new data or load"
                        " an existing file") != 0) {
                break;
            }
            tui_action_saveas(dataset, cur_filename);
            break;

        case TUI_MENU_SHOW_TABLE:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data entered: enter new data or load"
                        " an existing file") != 0) {
                break;
            }
            tui_action_show_data(dataset);
            break;

        case TUI_MENU_PLOT:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data to plot: enter new data or load"
                        " an existing file") != 0) {
                break;
            }
            tui_action_plot(dataset);
            break;

        case TUI_MENU_STATISTICS:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data to analyze: enter new data or load"
                        " an existing file") != 0) {
                break;
            }
            tui_action_stats(dataset);
            break;

        case TUI_MENU_REGRESSION:
            if (tui_dialog_alert_on_condition(dataset_size(dataset),
                        "No data to analyze: enter new data or load"
                        " an existing file") != 0) {
                break;
            }
            tui_action_regres(dataset);
            break;

        case TUI_MENU_ABOUT:
            tui_action_about();
            break;

        case TUI_MENU_QUIT:
            if (!tui_dialog_confirm_if_modified(
                        dataset_is_modified(dataset),
                        "Unsaved data! Exit anyway? (y/N)") != 0) {
                break;
            }
            *is_running = 0;
            break;
    }
}
