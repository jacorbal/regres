/**
 * @file tui/menu.h
 *
 * @brief Declaration of menu routines
 */

#ifndef TUI_MENU_H
#define TUI_MENU_H


/* Library includes */
#include <menu.h>
#include <ncurses.h>


/**
 * @brief Main menu item labels
 */
typedef enum {
    TUI_MENU_INPUT_DATA,
    TUI_MENU_LOAD_DATA,
    TUI_MENU_SAVE_DATA,
    TUI_MENU_SAVEAS_DATA,
    TUI_MENU_SHOW_TABLE,
    TUI_MENU_PLOT,
    TUI_MENU_STATISTICS,
    TUI_MENU_REGRESSION,
    TUI_MENU_ABOUT,
    TUI_MENU_QUIT,
    TUI_MENU_MAX
} menu_choice_e;


/* Public interface */
/**
 * @brief Create a menu for the terminal user interface
 * 
 * Initializes a menu window, creates menu items based on predefined
 * choices, and sets up the menu for user interaction.  It also handles
 * the case where the dataset is empty by disabling certain menu
 * options.
 * 
 * @param out_menu_win    Pointer to a window pointer where the menu
 *                        window will be stored
 * @param out_menu_sub    Pointer to a window pointer where the submenu
 *                        window will be stored
 * @param out_items       Pointer to an item pointer where the created 
 *                        items will be stored
 * @param out_items_count Pointer to an integer where the count of 
 *                        created items will be stored
 * @param dataset         Pointer to the dataset structure used for menu
 *                        options
 * @param cur_filename    Pointer to the current filename string
 *
 * @return Pointer to the created menu, or @c NULL if an error occurs
 *         during creation
 */
MENU *tui_menu_create(WINDOW **out_menu_win, WINDOW **out_menu_sub,
        ITEM ***out_items, int *out_items_count,
        dataset_td *dataset, const char *cur_filename);
/**
 * @brief Clean up and frees resources used by the menu
 * 
 * Unposts and frees the menu, deallocates memory for the menu items,
 * and deletes the associated windows.
 * 
 * @param menu        Pointer to the menu structure to be destroyed
 * @param menu_win    Pointer to the main menu window to be deleted
 * @param menu_sub    Pointer to the submenu window to be deleted
 * @param items       Pointer to the array of menu items to be freed
 * @param items_count Number of items in the menu
 */
void tui_menu_destroy(MENU *menu, WINDOW *menu_win, WINDOW *menu_sub,
        ITEM **items, int items_count);

/**
 * @brief Navigate the menu and retrieves the selected index
 * 
 * Allows the user to navigate through the menu using keyboard input and
 * returns the index of the currently selected item when the @c Enter
 * key is pressed.
 * 
 * @param menu     Pointer to the @e MENU structure being navigated
 * @param menu_win Pointer to the window displaying the menu
 *
 * @return int The index of the selected menu item
 */
int tui_menu_navigate_and_get_index(MENU *menu, WINDOW *menu_win);

/**
 * @brief Execute the action corresponding to the selected menu item
 * 
 * Takes the index of the selected menu item and executes the
 * corresponding action, which may include loading, saving, or
 * displaying data, as well as quitting the application.
 * 
 * @param index        Index of the selected menu item
 * @param dataset      Pointer to the dataset structure used in actions
 * @param cur_filename Pointer to the current filename string
 * @param is_running   Pointer to an integer indicating if the TUI 
 *                     should continue running
 */
void tui_menu_execute_choice(int index, dataset_td *dataset,
        char **cur_filename, int *is_running);


#endif  /* ! TUI_MENU_H */
