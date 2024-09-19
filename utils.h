#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>  // Include for getpwuid and struct passwd

void print_permissions(mode_t mode);

int compare_mod_time(const void *a, const void *b);
int compare_acc_time(const void *a, const void *b);
int compare_chg_time(const void *a, const void *b);

void do_ls(char* directories[], int dir_count, int long_list, int show_all, int do_not_sort, int sort_option, int one_per_line, int show_inode, int list_directories_only);

#endif // FILE_OPERATIONS_H

