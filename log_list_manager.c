#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_list_manager.h"

static int free_entry(LogEntry *log_e);

/*
* Creates and initializes a new LogManager.
*/
LogManager *create_log_manager(){

    LogManager *log_m = calloc(1, sizeof(LogManager));
    if(log_m == NULL){
        return NULL;
    }
    return log_m;

}  

/*
* Frees all memory associated with the LogManager, including all log entries
* and their dynamically allocated strings.
*/
int destroy_log_manager(LogManager *log_m) {

    LogEntry *curr_entry = log_m->head, *next_entry = NULL;
    /* Invalid Parameters */
    if(log_m == NULL){
        return FAILURE;
    }
    while(curr_entry != NULL){
        next_entry = curr_entry->next;
        free_entry(curr_entry);
        curr_entry = next_entry;
    }
    log_m->head = NULL;
    free(log_m);
    return SUCCESS;

}

/*
* Frees memory associated with a log
*/
static int free_entry(LogEntry *log_e){

    /* Invalid Parameters */
    if(log_e == NULL){
        return FAILURE;
    }
    if(log_e->time_stamp != NULL){
        free(log_e->time_stamp);
    }
    if (log_e->severity != NULL){
        free(log_e->severity);
    }
    if(log_e->message != NULL){
        free(log_e->message);
    }
    free(log_e);
    
    return SUCCESS;
}

/*
* Adds a new log entry to the end of the linked list.
*/
int add_log(LogManager *log_m, const char *timestamp, const char *severity, const char *message) {
    
    LogEntry *curr_entry = log_m->head, *new_entry;
    /* Invalid Parameters */
    if(log_m == NULL || timestamp == NULL || severity == NULL || message == NULL){
        return FAILURE;
    }  

    new_entry = calloc(1, sizeof(LogEntry));
    if(new_entry == NULL){
        return FAILURE;
    }
    new_entry->message = malloc(strlen(message) + 1);
    if(new_entry->message == NULL){
        return FAILURE;
    }
    strcpy(new_entry->message, message);
    new_entry->severity = malloc(strlen(severity) + 1);
    if(new_entry->severity == NULL){
        return FAILURE;
    }
    strcpy(new_entry->severity, severity);
    new_entry->time_stamp = malloc(strlen(timestamp) + 1);
    if(new_entry->time_stamp == NULL){
        return FAILURE;
    }
    strcpy(new_entry->time_stamp, timestamp);

    /* Case where we are adding the first element */
    if(curr_entry == NULL){
        log_m->head = new_entry;
        log_m->count++;
        return SUCCESS;
    }

    while(curr_entry->next != NULL){
        curr_entry = curr_entry->next;
    }
    log_m->count++;
    curr_entry->next = new_entry;
    return SUCCESS;
}

/*
* Removes all log entries that match the specified severity (case sensitive).
* Frees memory associated with those entries.
*/
int remove_logs_by_severity(LogManager *log_m, const char *severity) {
    
    int num_entry = 0;
    LogEntry *curr_entry = log_m->head, *prev_entry = NULL;
    /* Invalid Parameters */
    if(log_m == NULL || severity == NULL){
        return FAILURE;
    }
    while (curr_entry != NULL){
        if(strcmp(curr_entry->severity, severity) == 0){
            /* Case where first entry is being removed */
            if(prev_entry == NULL){
                log_m->head = curr_entry->next;
                free_entry(curr_entry);
                curr_entry = log_m->head;
            }else{
                prev_entry->next = curr_entry->next;
                free_entry(curr_entry);
                curr_entry = prev_entry->next;
            }
            num_entry++;
            log_m->count--;
        }else{
            curr_entry = curr_entry->next;
        }
    }

    return num_entry;

}

/*
* Outputs all logs to the specified stream in the format:
* [timestamp] [SEVERITY] message\n
*/
int write_all_logs(const LogManager *log_m, FILE *stream){
    
    LogEntry *curr_entry = log_m->head;
    /* Invalid Parameters */
    if(log_m == NULL || stream == NULL){
        return FAILURE;
    }
    while (curr_entry != NULL){
        fprintf(stream, "[%s] [%s] %s\n", curr_entry->time_stamp, curr_entry->severity, curr_entry->message);
        curr_entry = curr_entry->next;
    }
    return SUCCESS;
    
}

/*
* Searches for logs containing a keyword in the message field.
* Outputs matching logs to the specified stream.
*/
int search_and_write_logs(const LogManager *log_m, const char *keyword, FILE *stream){

    LogEntry *curr_entry = log_m->head;
    int num_entries = 0;
    /* Invalid Parameters */
    if(log_m == NULL || keyword == NULL || stream == NULL){
        return FAILURE;
    }
    while (curr_entry != NULL){
        if(strstr(curr_entry->message, keyword) != NULL){
            fprintf(stream, "[%s] [%s] %s\n", curr_entry->time_stamp, curr_entry->severity, curr_entry->message);
            num_entries++;
        } 
        curr_entry = curr_entry->next;
    }
    
    return num_entries;
}

/*
* Clears all log entries, freeing their memory, but keeps the LogManager usable.
*/
int clear_logs(LogManager *log_m){

    LogEntry *curr_entry = log_m->head, *next_entry = NULL;
    /* Invalid Parameters */
    if(log_m == NULL){
        return FAILURE;
    }
    while (curr_entry != NULL){
        next_entry = curr_entry->next;
        free_entry(curr_entry);
        curr_entry = next_entry;
    }
    log_m->head = NULL;
    return SUCCESS;

}
