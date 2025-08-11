#ifndef LOG_ARRAY_MANAGER_H
#define LOG_ARRAY_MANAGER_H

#include <stdio.h>

#define SUCCESS 0
#define FAILURE -1

/* Structure representing a single log entry */
typedef struct {
    char *time_stamp; /* YYYY-MM-DD HH:MM */
    char *severity;
    char *message;
} LogEntry;

/* Structure representing a dynamic array of log entries */
typedef struct {
    LogEntry **entries;  /* Array of pointers to LogEntry */
    size_t count;        /* Number of current entries */
    size_t capacity;     /* Current capacity of the array */
} LogArrayManager;


LogArrayManager *create_log_array_manager(size_t initial_capacity);
int destroy_log_array_manager(LogArrayManager *log_am);
int add_log_entry(LogArrayManager *log_am, const char *timestamp, const char *severity, const char *message);
int remove_logs_by_severity(LogArrayManager *log_am, const char *severity);
int clear_log_array(LogArrayManager *log_am);
int write_all_logs(const LogArrayManager *log_am, FILE *stream);
int quick_sort_array(LogArrayManager *log_am);
int merge_sort_array(LogArrayManager *log_am);
int heap_sort_array(LogArrayManager *log_am);

#endif 
