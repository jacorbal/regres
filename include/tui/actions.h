/**
 * @file tui/actions.h
 *
 * @brief Actions routines declaration
 */

#ifndef TUI_ACTIONS_H
#define TUI_ACTIONS_H


/* System includes */
#include <linux/limits.h>   /* PATH_MAX */

/* Library includes */
#include <ncurses.h>

/* Project includes */
#include <dataset.h>


#ifndef PATH_MAX
#define PATH_MAX (4096)
#endif  /* ! PATH_MAX */


/* Public interface */
/**
 * @brief Handle input action for the dataset
 *
 * Creates a new window for user input and calls the function to view
 * and input data into the dataset.
 *
 * @param dataset Pointer to the dataset structure to be modified
 */
void tui_action_input(dataset_td *dataset);

/**
 * @brief Handle load action for the dataset
 *
 * Creates a new window for loading a dataset from a specified filename.
 * It prompts the user for the filename and attempts to load the data
 * into the dataset.
 *
 * @param dataset  Pointer to the dataset structure to be loaded
 * @param filename Pointer to the current filename string
 */
void tui_action_load(dataset_td *dataset, char **filename);

/**
 * @brief Handle save action for the dataset
 *
 * Prompts the user to confirm saving changes to the current dataset.
 * If confirmed, it attempts to save the dataset to the current
 * filename.
 *
 * @param dataset  Pointer to the dataset structure to be saved
 * @param filename Pointer to the current filename string
 */
void tui_action_save(dataset_td *dataset, char **filename);

/**
 * @brief Handle save as action for the dataset
 *
 * Creates a new window for the user to specify a new filename to save
 * the dataset.  It attempts to save the data to the new filename
 * provided by the user.
 *
 * @param dataset  Pointer to the dataset structure to be saved
 * @param filename Pointer to the current filename string
 */
void tui_action_saveas(dataset_td *dataset, char **filename);

/**
 * @brief Show the data in the dataset
 *
 * Creates a new window to display the data points contained in the
 * dataset.
 *
 * @param dataset Pointer to the dataset structure to be displayed
 */
void tui_action_show_data(const dataset_td *dataset);

/**
 * @brief Plot the data from the dataset
 *
 * Performs linear regression on the dataset and plots the resulting
 * data.
 *
 * @param dataset Pointer to the dataset structure to be plotted
 */
void tui_action_plot(const dataset_td *dataset);

/**
 * @brief Compute and show statistics for the dataset
 *
 * Computes statistics for the dataset and displays the results in a new
 * window.
 *
 * @param dataset Pointer to the dataset structure for which statistics
 *                are computed
 */
void tui_action_stats(const dataset_td *dataset);

/**
 * @brief Show regression analysis for the dataset
 *
 * Performs linear regression on the dataset and displays the regression
 * results in a new window.
 *
 * @param dataset Pointer to the dataset structure for regression
 *                analysis
 */
void tui_action_regres(const dataset_td *dataset);

/**
 * @brief Show information about the program
 *
 * Creates a new window to display information about the program and its
 * functionalities.
 */
void tui_action_about(void);


#endif  /* ! TUI_ACTIONS_H */
