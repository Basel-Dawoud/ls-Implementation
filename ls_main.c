#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void print_usage() {
    printf("Usage: doLs [options] [directory...]\n");
    printf("Options:\n");
    printf("  -l          Use long listing format\n");
    printf("  -a          Show all files, including hidden\n");
    printf("  -t          Sort by modification time\n");
    printf("  -u          Sort by access time\n");
    printf("  -c          Sort by change time\n");
    printf("  -f          Do not sort\n");
    printf("  -d          List directory entries instead of contents\n");
    printf("  -1          List one file per line\n");
    printf("  -i          Show inode numbers\n");
    printf("  --color     Colorize the output\n");
}

int main(int argc, char *argv[]) {
    int long_list = 0, show_all = 0, do_not_sort = 0, one_per_line = 0, show_inode = 0;
    int sort_option = 0; // 0 = default, 1 = mod time, 2 = acc time, 3 = chg time
    int list_directories_only = 0; // Flag for -d option
    char *directories[256]; // Array to hold directory names
    int dir_count = 0; // Number of directories

    int opt;
    while ((opt = getopt(argc, argv, "latucifd1")) != -1) {
        switch (opt) {
            case 'l':
                long_list = 1;
                break;
            case 'a':
                show_all = 1;
                break;
            case 'i':
                show_inode = 1;
                break;
            case 't':
                sort_option = 1;
                break;
            case 'u':
                sort_option = 2;
                break;
            case 'c':
                sort_option = 3;
                break;
            case 'f':
                do_not_sort = 1;
                break;
            case 'd':
                list_directories_only = 1; // Set the flag for -d option
                break;
            case '1':
                one_per_line = 1;
                break;
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    // Remaining arguments are directories
    for (int i = optind; i < argc; i++) {
        directories[dir_count++] = argv[i];
    }
    
    if (dir_count == 0) {
        directories[dir_count++] = "."; // Default to current directory
    }

    // Call the function to list directories
    do_ls(directories, dir_count, long_list, show_all, do_not_sort, sort_option, one_per_line, show_inode, list_directories_only);
    
    return 0;
}

