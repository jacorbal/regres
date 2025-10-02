/**
 * @file tui/dialogs.h
 *
 * @brief Declaration of dialogs routines
 */

#ifndef TUI_DIALOGS_H
#define TUI_DIALOGS_H


/* System includes */
#include <linux/limits.h>   /* PATH_MAX */
#include <stddef.h>         /* size_t */


#ifndef PATH_MAX
#define PATH_MAX (4096)
#endif  /* ! PATH_MAX */


/* Public interface */
/**
 * @brief Present a message if data size is zero
 *
 * @param condition If zero, prompt message
 * @param msg       Message to show on error
 *
 * @return 0 if data set is not empty, 1 otherwise
 */
int tui_dialog_alert_on_condition(int condition, const char *msg);

/**
 * @brief Show a modal dialog asking whether to do something when data
 *        has unsaved changes
 *
 * Presents a modal confirmation dialog if the provided dataset is
 * marked as modified.  The dialog asks the user whether to exit without
 * saving.
 *
 * @param is_modified If 1, prompt question
 *
 * @return 1 if the caller should proceed with exiting (user confirmed);
 *         0 if the exit should be cancelled (user declined)
*/
int tui_dialog_confirm_if_modified(int is_modified, const char *msg);


#endif  /* ! TUI_DIALOGS_H */
