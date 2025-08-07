#include <stdio.h>
#include <stdlib.h>
#include "log_array_manager.h"

int main(void) {
    LogArrayManager *log_manager = create_log_array_manager(5);
    if (!log_manager) {
        fprintf(stderr, "Failed to create log manager\n");
        return 1;
    }

    add_log_entry(log_manager, "2025-07-22 12:00", "INFO", "System started");
    add_log_entry(log_manager, "2025-07-22 12:01", "WARN", "Low disk space");
    add_log_entry(log_manager, "2025-07-22 12:02", "ERROR", "Disk full");
    add_log_entry(log_manager, "2025-07-22 12:03", "INFO", "User login");
    add_log_entry(log_manager, "2025-07-22 12:04", "ERROR", "Network failure");

    printf("Logs before sorting:\n");
    write_all_logs(log_manager, stdout);

    merge_sort_array(log_manager);

    printf("\nLogs after merge sort:\n");
    write_all_logs(log_manager, stdout);

    remove_logs_by_severity(log_manager, "WARN");

    printf("\nAfter removing WARN logs:\n");
    write_all_logs(log_manager, stdout);

    clear_log_array(log_manager);

    printf("\nAfter clearing logs:\n");
    write_all_logs(log_manager, stdout);

    destroy_log_array_manager(log_manager);

    return 0;
}
