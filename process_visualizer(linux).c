#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUF 1024

// Function to check if a string is numeric
int isNumeric(const char *str) {
    while (*str) {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

// Function to parse and display process information
void displayProcessInfo(const char *pid) {
    char path[MAX_BUF], line[MAX_BUF], *p;
    FILE *fp;

    // Construct the path to the process's status file in /proc
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    // Open the process status file
    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return;
    }

    // Read and display process information
    printf("PID: %s\n", pid);
    while (fgets(line, sizeof(line), fp)) {
        if ((p = strchr(line, '\n')) != NULL)
            *p = '\0'; // Remove newline character
        printf("%s\n", line);
    }

    // Close the file
    fclose(fp);
}

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the /proc directory
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        return 1;
    }

    // Iterate over the contents of /proc
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a directory and its name is numeric (representing a PID)
        if (entry->d_type == DT_DIR && isNumeric(entry->d_name)) {
            // Display process information
            displayProcessInfo(entry->d_name);
            printf("\n");
        }
    }

    // Close the /proc directory
    closedir(dir);

    return 0;
}
