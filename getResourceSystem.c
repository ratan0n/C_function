#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_cpu_usage() {
    printf("CPU Usage:\n");
    system("wmic cpu get loadpercentage");
}

void get_memory_usage() {
    printf("Memory Usage:\n");
    FILE *fp;
    char output[1024];
    // Run the command and capture the output
    fp = _popen("wmic OS get FreePhysicalMemory,TotalVisibleMemorySize /Value", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    // Read the output a line at a time
    while (fgets(output, sizeof(output), fp) != NULL) {
        if (strstr(output, "FreePhysicalMemory") != NULL) {
            unsigned long long freeMemory;
            sscanf(output, "FreePhysicalMemory=%llu", &freeMemory);
            printf("Free Physical Memory: %.2f GB\n", freeMemory / 1024.0 / 1024.0);
        } else if (strstr(output, "TotalVisibleMemorySize") != NULL) {
            unsigned long long totalMemory;
            sscanf(output, "TotalVisibleMemorySize=%llu", &totalMemory);
            printf("Total Visible Memory Size: %.2f GB\n", totalMemory / 1024.0 / 1024.0);
        }
    }
    // Close the pipe
    _pclose(fp);
}

void get_disk_usage() {
    printf("Disk Usage:\n");
    FILE *fp;
    char output[1024];
    // Run the command and capture the output
    fp = _popen("wmic logicaldisk get size,freespace,caption", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    // Read the output a line at a time and parse the output
    while (fgets(output, sizeof(output), fp) != NULL) {
        char caption[10];
        unsigned long long size, freeSpace;
        if (sscanf(output, "%s %llu %llu", caption, &freeSpace, &size) == 3) {
            printf("%s Free Space: %.2f GB\n", caption, freeSpace / 1024.0 / 1024.0 / 1024.0);
            printf("%s Size: %.2f GB\n", caption, size / 1024.0 / 1024.0 / 1024.0);
        }
    }
    // Close the pipe
    _pclose(fp);
}

void get_disk_io() {
    printf("Disk I/O:\n");
    printf("Windows does not provide a simple command line tool for disk I/O statistics like iostat on Linux.\n");
    printf("Consider using Performance Monitor (perfmon.exe) or typeperf commands for detailed I/O stats.\n");
}

int main() {
    get_cpu_usage();
    get_memory_usage();
    get_disk_usage();
    get_disk_io();

    return 0;
}
