/**
 * @file tui.h
 *
 * @brief Declaration of TUI routines
 */

#ifndef TUI_H
#define TUI_H


/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <global.h>


/* Public interface */
/**
 * @brief Start the TUI
 * 
 * Initializes the terminal user interface (TUI) and checks if the
 * terminal size is sufficient. If the terminal is too small, it
 * displays an error message and exits.
 * 
 * @return 0 if the TUI starts successfully, or 1 if the terminal size
 *         is too small
 */
int tui_start(void);

/**
 * @brief Main event loop for the TUI
 * 
 * Runs the main event loop of the TUI, handling user interactions, menu
 * navigation, and executing selected actions until the TUI is
 * terminated.
 * 
 * @return 0 when the event loop ends, or otherwise
 */
int tui_loop(void);

/**
 * @brief Finishes the TUI
 * 
 * Restores the cursor visibility and ends the terminal user interface,
 * cleaning up any resources used
 */
void tui_end(void);


#endif  /* ! TUI_H */
