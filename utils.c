#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>  // For getpwuid and struct passwd
#include <libgen.h>

#define MAX_FILENAME_LEN 512  // Increased size for full path

typedef struct {
    char name[MAX_FILENAME_LEN];  // Adjusted size for file path
    struct stat info;
} FileInfo;

void print_permissions(mode_t mode) {
    char str[11] = "----------";
    
    // Owner permissions
    if (mode & S_IRUSR) str[0] = 'r';
    if (mode & S_IWUSR) str[1] = 'w';
    if (mode & S_IXUSR) str[2] = (mode & S_ISUID) ? 's' : 'x';
    else if (mode & S_ISUID) str[2] = 'S';

    // Group permissions
    if (mode & S_IRGRP) str[3] = 'r';
    if (mode & S_IWGRP) str[4] = 'w';
    if (mode & S_IXGRP) str[5] = (mode & S_ISGID) ? 's' : 'x';
    else if (mode & S_ISGID) str[5] = 'S';

    // Other permissions
    if (mode & S_IROTH) str[6] = 'r';
    if (mode & S_IWOTH) str[7] = 'w';
    if (mode & S_IXOTH) str[8] = (mode & S_ISVTX) ? 't' : 'x';
    else if (mode & S_ISVTX) str[8] = 'T';

    str[9] = '\0'; // Null-terminate
    printf("%s ", str);
}

int compare_mod_time(const void *a, const void *b) {
    return ((FileInfo*)b)->info.st_mtime - ((FileInfo*)a)->info.st_mtime;
}

int compare_acc_time(const void *a, const void *b) {
    return ((FileInfo*)b)->info.st_atime - ((FileInfo*)a)->info.st_atime;
}

int compare_chg_time(const void *a, const void *b) {
    return ((FileInfo*)b)->info.st_ctime - ((FileInfo*)a)->info.st_ctime;
}

void do_ls(char* directories[], int dir_count, int long_list, int show_all, int do_not_sort, int sort_option, int one_per_line, int show_inode, int list_directories_only) {
    for (int i = 0; i < dir_count; i++) {
        // If -d option is specified, print directory name only
        if (list_directories_only) {
            if (long_list) {
                // Optionally, you could show permissions or other details here
                printf("Directory: %s\n", directories[i]);
            } else {
                printf("%s\n", directories[i]);
            }
            continue; // Skip listing contents for this directory
        }

        struct dirent *entry;
        DIR *dp = opendir(directories[i]);

        if (dp == NULL) {
            perror("Error opening directory");
            continue;
        }

        printf("Directory: %s\n", directories[i]);

        FileInfo files[256];
        int file_count = 0;

        while ((entry = readdir(dp)) != NULL) {
            if (!show_all && entry->d_name[0] == '.') {
                continue; // Skip hidden files if -a not set
            }

            snprintf(files[file_count].name, sizeof(files[file_count].name), "%s/%s", directories[i], entry->d_name);
            if (lstat(files[file_count].name, &files[file_count].info) == -1) {
                perror("Error getting file info");
                continue;
            }
            file_count++;
        }

        closedir(dp);

        // Sort files if not using -f option
        if (!do_not_sort) {
            switch (sort_option) {
                case 1: // Sort by modification time
                    qsort(files, file_count, sizeof(FileInfo), compare_mod_time);
                    break;
                case 2: // Sort by access time
                    qsort(files, file_count, sizeof(FileInfo), compare_acc_time);
                    break;
                case 3: // Sort by change time
                    qsort(files, file_count, sizeof(FileInfo), compare_chg_time);
                    break;
                default:
                    break;
            }
        }

        // Display the files
        for (int j = 0; j < file_count; j++) {
            if (long_list) {
                if (show_inode) {
                    printf("%lu ", files[j].info.st_ino); // Display inode number
                }
                print_permissions(files[j].info.st_mode);
                
                // Display additional info
                printf("%ld ", files[j].info.st_nlink); // Number of links
                
                // Owner name
                struct passwd *pw = getpwuid(files[j].info.st_uid);
                printf("%s ", pw ? pw->pw_name : "UNKNOWN");

                // Group name
                struct group *gr = getgrgid(files[j].info.st_gid);
                printf("%s ", gr ? gr->gr_name : "UNKNOWN");

                // File size
                printf("%ld ", files[j].info.st_size);

                // Last modified date and time
                char timebuf[80];
                struct tm *tm_info = localtime(&files[j].info.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
                printf("%s ", timebuf);

                // File name (only name, not full path)
                printf("%s\n", basename(files[j].name)); // Use basename to extract the file name
            } else {
                // Only print the file name without full path
                printf("%s", basename(files[j].name));
                if (one_per_line) {
                    printf("\n");
                } else {
                    printf("  ");
                }
            }
        }
        
        if (!one_per_line) {
            printf("\n"); // Newline after each directory listing if not in one-per-line mode
        }
    }
}

