#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_array_manager.h"

static int free_entry(LogEntry *log_e); 
static int create_entry(LogEntry **log_e, const char *timestamp, const char *severity, const char *message);

/*
* Creates and initializes a new LogArrayManager with initial capacity.
* Returns pointer to the LogArrayManager on success, NULL on failure.
*/
LogArrayManager *create_log_array_manager(size_t initial_capacity){

    LogArrayManager *log_am = calloc(1, sizeof(LogArrayManager));
    if(log_am == NULL){
        return NULL;
    }
    log_am->capacity = initial_capacity;
    log_am->entries = calloc(initial_capacity, sizeof(LogEntry *));
    if(log_am->entries == NULL){
        free(log_am);
        return NULL;
    }
    return log_am;

}

/*
* Frees all memory associated with the LogArrayManager,
* including all log entries and their dynamically allocated strings.
* Returns SUCCESS on success, FAILURE on invalid input.
*/
int destroy_log_array_manager(LogArrayManager *log_am){

    size_t i;
    /* Invalid Parameters */
    if(log_am == NULL){
        return FAILURE;
    }
    for (i = 0; i < log_am->count; i++){
       free_entry(log_am->entries[i]);
       log_am->entries[i] = NULL;
    }
    free(log_am->entries);
    free(log_am);
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
* Adds a log entry to the array, resizing (realloc) if necessary.
*/
int add_log_entry(LogArrayManager *log_am, const char *timestamp, const char *severity, const char *message){

    /* Invalid Parameters */
    if(log_am == NULL || timestamp == NULL || severity == NULL || message == NULL){
        return FAILURE;
    }
    LogEntry *log_e = NULL;
    if(create_entry(&log_e, timestamp, severity, message) == FAILURE) return FAILURE;
    /* case where we do NOT need reallocating memory */
    if(log_am->count < log_am->capacity){
        log_am->entries[log_am->count] = log_e;
        log_am->count++;
    }
    /* case where we do need to reallocate memory */
    else{  
        /* doubles the capacity */
        LogEntry **test_ptr= realloc(log_am->entries, (log_am->capacity * 2) * sizeof(LogEntry *));
        if(test_ptr == NULL){
            return FAILURE;
        }else{
            log_am->entries = test_ptr;
            log_am->capacity *= 2;
        }
        log_am->entries[log_am->count] = log_e;
        log_am->count++;
    }
    return SUCCESS;
}   

/*
* Creates a dynamically allocated log entry structre using the provided parameters
* (assuming that all parameters are valid)
*/
static int create_entry(LogEntry **log_e, const char *timestamp, const char *severity, const char *message){

    *log_e = malloc(sizeof(LogEntry));
    if(*log_e == NULL){
        return FAILURE;
    }

    (*log_e)->time_stamp = malloc(strlen(timestamp) + 1);
    if((*log_e)->time_stamp == NULL){
        return FAILURE;
    }
    strcpy((*log_e)->time_stamp, timestamp);

    (*log_e)->severity = malloc(strlen(severity) + 1);
    if((*log_e)->severity == NULL){
        return FAILURE;
    }
    strcpy((*log_e)->severity, severity);

    (*log_e)->message = malloc(strlen(message) + 1);
    if((*log_e)->message == NULL){
        return FAILURE;
    }
    strcpy((*log_e)->message, message);

    return SUCCESS;
}

/*
 * Removes all log entries matching the given severity.
 */
int remove_logs_by_severity(LogArrayManager *log_am, const char *severity){
    size_t i;
    int num_entries_removed = 0;
    for(i = 0; i < log_am->count; i++){
        LogEntry *log_e = log_am->entries[i];
        if(strcmp(log_e->severity, severity) == 0){ /* checks to see if severity are matched */
            memmove(log_am->entries + i, log_am->entries + (i+1) , (log_am->count - (i+1)) * sizeof(LogEntry *));
            log_am->count--;
            i--;
            num_entries_removed++;
        }
    }
    return num_entries_removed;
}

/*
 * Clears all log entries and resets the manager to empty state.
 */
int clear_log_array(LogArrayManager *log_am){
    size_t i;
    for(i = 0; i < log_am->count; i++){
        LogEntry *log_e = log_am->entries[i];
        /* frees the memory allocated for our log_e */
        free_entry(log_e);
        log_am->entries[i] = NULL; /* takes it off of our manager */
    }
    log_am->count = 0;
    return SUCCESS;
}

/*
 * Writes all logs to the given stream.
 * Returns SUCCESS on success, FAILURE on error.
 */
int write_all_logs(const LogArrayManager *log_am, FILE *stream){
    size_t i;
    for(i = 0; i < log_am->count; i++){
        LogEntry *log_e = log_am->entries[i];
        fprintf(stream, "[%s] [%s] %s\n", log_e->time_stamp, log_e->severity, log_e->message);
    }
    return SUCCESS;
}