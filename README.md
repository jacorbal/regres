Regres -- Linear regression analysis tool
=========================================

## Overview

Regres is a **linear regression analysis tool** developed using the
`ncurses` library.  It allows users to perform linear regression
analysis using the least squares method, visualize data, and conduct
statistical analysis in a terminal-based user interface (TUI).

## History

It was initially developed during the laboratory practices for various
first-year Physics courses at the University of Salamanca (Spain) in
2002.  It quickly became an essential tool for many students.  The first
version was created in a rudimentary format for DOS, while the third
iteration introduced graphical representation capabilities using an SVGA
controller (`BGI`).  That code still exists, but it's not public.

This current version is a simplified edition, as it does not allow data
editing (yet) nor does it account for multiple measurements for the same
value as previous versions.  However, it has finally been adapted for
Linux, featuring a menu interface built with `ncurses`.

## Version

  - Current: version 4.1.1 (with further improvements yet to come)

## Features

  - Data input and storage
  - Load and save datasets from files
  - Visualize data through plotting (using `gnuplot`)
  - Display statistical information
  - Perform linear regression analysis

## Requirements

  - C compiler compliant with the ISO/IEC 9899:1999 standard (C99):
    `gcc`, `clang`, etc.
  - `ncurses` library
  - `gnuplot` [optional], but the plotting option invokes `gnuplot`

## Installation

### 1. Clone the repository

    $ git clone https://github.com/jacorbal/regres.git
    $ cd regres

### 2. Compile the program

    $ make

### 3. Run the program

    $ cp ./bin/main ~/.local/bin/regres
    $ regres

## Usage

  - **Input data,**  Select *Input new data* from the main menu to input
    your dataset.
  - **Load data.**  Select *Load data from file* to load a previously
    saved dataset.
  - **Save data.**  Select *Save current data* or *Save as* to store
    your dataset.
  - **Perform analysis.**
    - Select *Statistics* to view statistical information about your
      dataset.
    - Select *Linear regression* to perform linear regression analysis.
  - **Plot data.**  Select *Plot graph* to visualize your data and
    regression line by invoking `gnuplot`.
  - **About.**  Select *About* to view information about the program.
  - **Quit.**  Select *Quit* to exit the program, with confirmation for
    unsaved changes.

## License

  - This project is licensed under the MIT License.
  - See the [`LICENSE`](LICENSE) file for details.
  - Copyright (c) 2002-2025, J. A. Corbal

## Contributing

Contributions are welcome.  Feel free to submit a pull request or open
an issue to discuss improvements or suggestions.  Take into
consideration that the development of this program is still ongoing.
