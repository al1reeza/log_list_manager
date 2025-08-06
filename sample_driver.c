#include <stdio.h>
#include <string.h>

#include "log_list_manager.h"

int main(void) {
    LogManager *log_m = create_log_manager();
    if (log_m == NULL) {
        printf("Failed to create LogManager\n");
        return 1;
    }

    add_log(log_m, "2025-07-06 10:00:00", "INFO", "System boot");
    add_log(log_m, "2025-07-06 10:05:00", "WARN", "Memory low");
    add_log(log_m, "2025-07-06 10:10:00", "INFO", "Process started");
    add_log(log_m, "2025-07-06 10:15:00", "ERROR", "Disk failure");

    printf("All logs (unsorted):\n");
    write_all_logs(log_m, stdout);

    printf("\nSorting logs by message (Bubble Sort)...\n");
    bubble_sort_logs(log_m);
    write_all_logs(log_m, stdout);

    add_log(log_m, "2025-07-06 10:20:00", "DEBUG", "Network initialized");
    add_log(log_m, "2025-07-06 10:25:00", "WARN", "CPU temperature high");

    printf("\nLogs after adding more entries (unsorted):\n");
    write_all_logs(log_m, stdout);

    printf("\nSorting logs by message (Merge Sort)...\n");
    merge_sort_logs(log_m);
    write_all_logs(log_m, stdout);

    printf("\nRemoving INFO logs...\n");
    int removed = remove_logs_by_severity(log_m, "INFO");
    printf("%d INFO logs removed.\n", removed);

    printf("\nRemaining logs:\n");
    write_all_logs(log_m, stdout);

    printf("\nClearing logs...\n");
    clear_logs(log_m);
    printf("After clearing:\n");
    write_all_logs(log_m, stdout);

    destroy_log_manager(log_m);

    return 0;
}


