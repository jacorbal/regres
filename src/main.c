/**
 * @file main.c
 *
 * @brief Main entry point
 *
 * Given a dataset of points, perform statistical analysis and linear
 * regression.  Data can be load & saved, and plot by using @c gnuplot
 * on a menu-driven @c ncurses interface.
 *
 * @author J. A. Corbal <jacorbal@gmail.com>
 * @version 4.1.2
 * @copyright Copyright (c) 2002-2025, J. A. Corbal.
 *            Licensed under MIT license; read `LICENSE` file for details
 *
 * @date Creation date: Thu Oct  2 11:03:54 UTC 2025
 * @date Last update: Thu Oct 11 13:52:17 UTC 2025
 */

/* System includes */
#include <stdio.h>      /* fprintf */

/* Project includes */
#include <tui.h>


/* Main entry */
int main(void)
{
    if (tui_start() != 0) {
        fprintf(stderr, "Cannot open TUI for Regres\n");
        return 1;
    }
    tui_loop();
    tui_end();

    return 0;
}
