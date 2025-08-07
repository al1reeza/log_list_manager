#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_list_manager.h"


static int free_entry(LogEntry *log_e);
static int list_comparator(const LogEntry *a, const LogEntry *b);
static LogEntry *merge_sort_aux(LogEntry *head, int len);
static LogEntry *merge(LogEntry *left_head, LogEntry *right_head);


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
    log_m->tail = NULL;
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
    
    LogEntry *new_entry;
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
        free(new_entry);
        return FAILURE;
    }
    strcpy(new_entry->message, message);
    new_entry->severity = malloc(strlen(severity) + 1);
    if(new_entry->severity == NULL){
        free(new_entry->message);
        free(new_entry);
        return FAILURE;
    }
    strcpy(new_entry->severity, severity);
    new_entry->time_stamp = malloc(strlen(timestamp) + 1);
    if(new_entry->time_stamp == NULL){
        free(new_entry->severity);
        free(new_entry->message);
        free(new_entry);
        return FAILURE;
    }
    strcpy(new_entry->time_stamp, timestamp);

    /* Case where we are adding the first element */
    if(log_m->head == NULL){
        log_m->head = new_entry;
        log_m->tail = new_entry;
        log_m->count++;
        return SUCCESS;
    }

    log_m->tail->next = new_entry;
    log_m->tail = new_entry;
    log_m->count++;

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
                if(log_m->tail == curr_entry){
                    log_m->tail = curr_entry->next;
                }
                free_entry(curr_entry);
                curr_entry = log_m->head;
            }
            /* Case where last entry is being removed */
            else if(curr_entry == log_m->tail){
                /* only entry */
                if(prev_entry == NULL){
                    log_m->tail = prev_entry;
                    log_m->head = prev_entry;
                    free_entry(curr_entry);
                    curr_entry = NULL;
                }else{
                    log_m->tail = prev_entry;
                    prev_entry->next = NULL;
                    free_entry(curr_entry);
                    curr_entry = NULL;
                }
                
            }
            /* Case where elemetn is in the middle */
            else{
                prev_entry->next = curr_entry->next;
                free_entry(curr_entry);
                curr_entry = prev_entry->next;
            }
            num_entry++;
            log_m->count--;
        }else{
            prev_entry = curr_entry;
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

/*
* Sorts the linked list using bubble sort 
*/
int bubble_sort_logs(LogManager *log_m){
    LogEntry *curr_entry, *prev_entry, *next_entry;
    int i;
    /* Invalid parameters */
    if(log_m == NULL || log_m->count < 2){
        return FAILURE;
    }
    for(i = 0; i < log_m->count-1; i++){
        int swapped = 0;

        curr_entry = log_m->head;
        prev_entry = NULL;
        next_entry = curr_entry->next;

        while(curr_entry != log_m->tail){
            if(list_comparator(curr_entry, next_entry) > 0){
                /* case we are swapping the head */
                if(prev_entry == NULL){
                    log_m->head = next_entry;
                    curr_entry->next = next_entry->next;
                    next_entry->next = curr_entry;
                }
                /* case we are swapping the tail */
                else if(next_entry == log_m->tail){
                    log_m->tail = curr_entry;
                    prev_entry->next = next_entry;
                    curr_entry->next = next_entry->next;
                    next_entry->next = curr_entry;
                }
                /* case we are swapping two thgins in the middle */
                else{
                    prev_entry->next = next_entry;
                    curr_entry->next = next_entry->next;
                    next_entry->next = curr_entry;
                }
                prev_entry = next_entry;
                next_entry = curr_entry->next;
                swapped = 1;
            }else{
                prev_entry = curr_entry;
                curr_entry = curr_entry->next;
                next_entry = next_entry->next;
            }
        }  

        if(!swapped){
            break;
        }

    }

    return SUCCESS;
}

/*
* Comparator for our list (based on message field of LogEntry).
*/
static int list_comparator(const LogEntry *a, const LogEntry *b){
    return strcmp(a->severity, b->severity);
}

/*
* Merge sort algorithm for linked list. 
*/
int merge_sort_logs(LogManager *log_m){
    LogEntry *curr_entry;
    /* Invalid Parameters */
    if(log_m == NULL || log_m->count < 2){
        return FAILURE;
    }
    log_m->head = merge_sort_aux(log_m->head, log_m->count); /* sorts the list */
    
    /* sets the tail */  
    curr_entry = log_m->head;
    while(curr_entry->next != NULL){
        curr_entry = curr_entry->next;
    }
    log_m->tail = curr_entry;

    return SUCCESS;
}

/*
* Merge sort auxillary function.
*/
static LogEntry *merge_sort_aux(LogEntry *head, int len){
    
    LogEntry *left_head = head, *right_head = head, *left_last;
    int i, middle = len/2;
    /* Base case */
    if(len <= 1){
        return head;
    } 
    /* Gets the right_head to the correct position */
    for(i = 0; i < middle; i++){
        if(i == middle - 1){
            left_last = right_head;
        }
        right_head = right_head->next;
    }

    left_last->next = NULL;

    left_head = merge_sort_aux(left_head, (len/2)); /* left subarray call */

    right_head = merge_sort_aux(right_head, len - (len/2)); /* right subarray call */

    return merge(left_head, right_head);
}

/*
* Merges the left and subarray sorted.
*/
static LogEntry *merge(LogEntry *left_head, LogEntry *right_head){

    LogEntry *curr_head, *head;

    /* Finds the head */
    if(list_comparator(left_head, right_head) <= 0){
        curr_head = left_head;
        left_head = left_head->next;
    }else{
        curr_head = right_head;
        right_head = right_head->next;
    }
    head = curr_head; /* sets the head for return */

    while(left_head != NULL && right_head != NULL){
        int cmp = list_comparator(left_head, right_head);
        if(cmp <= 0){
            curr_head->next = left_head;
            left_head = left_head->next;
            curr_head = curr_head->next;
        }else{
            curr_head->next = right_head;
            right_head = right_head->next;
            curr_head = curr_head->next;
        }
    }
    /* checks for any left over in left sublist */
    while (left_head != NULL){
        curr_head->next = left_head;
        left_head = left_head->next;
        curr_head = curr_head->next;
    }
    /* checks for any left over in right sublist */  
    while(right_head != NULL){
        curr_head->next = right_head;
        right_head = right_head->next;
        curr_head = curr_head->next;
    }

    return head;
}
