# doLs - A Custom Directory Listing Program
Implement full fledged ls utility with the options: -latucifd1

## Overview

`doLs` is a command-line program that mimics the functionality of the Unix/Linux `ls` command, allowing users to list directory contents with various options for formatting, sorting, and displaying file information. This implementation is designed to be modular, separating the main logic from utility functions.

## Features

- List files and directories in long format or simple format.
- Show hidden files.
- Sort files based on modification time, access time, or change time.
- Option to not sort the output.
- List only directory entries.
- Display inode numbers.
- Option to format output as one file per line.
- Colorize output (to be implemented).

## Program Structure

The program consists of the following files:

- `main.c`: The entry point of the program, responsible for parsing command-line arguments and invoking the listing function.
- `utils.c`: Contains the implementation of file listing, sorting, and permission printing functionalities.
- `utils.h`: Header file that declares the functions implemented in `utils.c`.

## Detailed Functionality

### 1. Command-Line Options

The program supports the following options:

- `-l`: Use long listing format.
- `-a`: Show all files, including hidden ones (files starting with a dot).
- `-t`: Sort files by modification time.
- `-u`: Sort files by access time.
- `-c`: Sort files by change time.
- `-f`: Do not sort files.
- `-d`: List directories themselves, not their contents.
- `-1`: List one file per line.
- `-i`: Show inode numbers.
- `--color`: Colorize the output (to be implemented).

### 2. Main Functionality

#### `main.c`

- The `main` function begins by defining flags and variables to track the state of command-line options and directories to be processed.
- It uses the `getopt` function to parse options and set corresponding flags.
- After parsing, it collects any directory names specified by the user or defaults to the current directory if none are provided.
- Finally, it calls the `do_ls` function to perform the directory listing.

#### `utils.c`

- **FileInfo Struct**: A structure to hold file names and their associated `stat` information.
- **print_permissions**: A function to print the file permissions in a human-readable format.
- **Comparison Functions**: Functions for sorting based on modification time, access time, or change time.
- **do_ls**: The main listing function that:
  - Handles the `-d` option to list directories only.
  - Reads directory entries and filters based on the `-a` option.
  - Sorts files as specified before printing them.
  - Displays files in long format or simple format, based on user options.

### 3. Example Usage

To compile the program:

```bash
gcc main.c utils.c -o doLs
```

To run the program with various options:

```bash
./doLs -l           # Long listing format
./doLs -a           # Show all files, including hidden
./doLs -t           # Sort by modification time
./doLs -1           # List one file per line
./doLs -d /path/to/directory  # List the directory itself
./doLs -i           # Show inode numbers
./doLs              # List current directory contents
```
[doLS.webm](https://github.com/user-attachments/assets/b9d87cbd-c0d2-4722-993e-c2c4eecdd8d9)

### 4. Future Improvements

- Add support for additional command-line options, such as sorting in ascending/descending order.
- Improve error handling for various edge cases (e.g., permission issues, empty directories).

Feel free to customize any section further to fit your project's specifics or add more details as needed!
