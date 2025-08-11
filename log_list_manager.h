#ifndef LOG_LIST_MANAGER_H
#define LOG_LIST_MANAGER_H

#include <stdio.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct LogEntry {
    char *time_stamp; /* YYYY-MM-DD HH:MM */
    char *severity;     
    char *message;      
    struct LogEntry *next; 
} LogEntry;


typedef struct LogManager {
    LogEntry *head; 
    LogEntry *tail; 
    int count;    
} LogManager;

LogManager *create_log_manager();
int destroy_log_manager(LogManager *log_m);
int add_log(LogManager *log_m, const char *timestamp, const char *severity, const char *message);
int remove_logs_by_severity(LogManager *log_m, const char *severity);
int write_all_logs(const LogManager *log_m, FILE *stream);
int search_and_write_logs(const LogManager *log_m, const char *keyword, FILE *stream);
int clear_logs(LogManager *log_m);
int bubble_sort_logs(LogManager *log_m);
int merge_sort_logs(LogManager *log_m); 

#endif 
