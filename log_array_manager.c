#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_array_manager.h"

static int free_entry(LogEntry *log_e); 
static int create_entry(LogEntry **log_e, const char *timestamp, const char *severity, const char *message);
static void quick_sort_array_aux(LogEntry **entries, int s_index, int p_index);
static void merge_sort_array_aux(LogArrayManager *log_am, int left, int right);
static int merge(LogEntry **entries, int left, int middle, int right);
static void build_max_heap(LogEntry **entries, int len);
static void bubble_down(LogEntry **entries, int idx, int len);
static void sort_max_heap(LogEntry **entries, int len);


/*
* Creates and initializes a new LogArrayManager with initial capacity.
* Returns pointer to the LogArrayManager on success, NULL on failure.
*/
LogArrayManager *create_log_array_manager(size_t initial_capacity){

    /* Invalid Parameter */
    if(initial_capacity == 0){
        return NULL;
    }
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

    LogEntry *log_e = NULL;
    /* Invalid Parameters */
    if(log_am == NULL || timestamp == NULL || severity == NULL || message == NULL){
        return FAILURE;
    }
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
        free(*log_e);
        return FAILURE;
    }
    strcpy((*log_e)->time_stamp, timestamp);

    (*log_e)->severity = malloc(strlen(severity) + 1);
    if((*log_e)->severity == NULL){
        free((*log_e)->time_stamp);
        free(*log_e);
        return FAILURE;
    }
    strcpy((*log_e)->severity, severity);

    (*log_e)->message = malloc(strlen(message) + 1);
    if((*log_e)->message == NULL){
        free((*log_e)->severity);
        free((*log_e)->time_stamp);
        free(*log_e);
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
    /* Invalid Parameters */
    if(log_am == NULL || severity == NULL){
        return FAILURE;
    }
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
    /* Invalid Parameters */
    if(log_am == NULL){
        return FAILURE;
    }
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
    if(log_am == NULL || stream == NULL){
        return FAILURE;
    }
    for(i = 0; i < log_am->count; i++){
        LogEntry *log_e = log_am->entries[i];
        fprintf(stream, "[%s] [%s] %s\n", log_e->time_stamp, log_e->severity, log_e->message);
    }
    return SUCCESS;
}

/*
* Comparator for our list (based on message field of LogEntry).
*/
static int array_comparator(const LogEntry *a, const LogEntry *b){
    return strcmp(a->severity, b->severity);
}

/*
* Quick sort for array.
*/
int quick_sort_array(LogArrayManager *log_am){
    
    /* Invalid Parameters */
    if(log_am == NULL || log_am->count < 2){
        return FAILURE;
    }
    quick_sort_array_aux(log_am->entries, 0, log_am->count - 1);
    return SUCCESS;

}

/*
* Auxillary function for quick_sort_array. (s in s_index refers to start)
*/
static void quick_sort_array_aux(LogEntry **entries, int s_index, int p_index){

    LogEntry *tmp;
    /* base case */
    if(s_index >= p_index){
        return;
    }
    
    int i = s_index - 1, j = s_index;
    /* puts all elements smaller than pivot in the begginging */
    while(j < p_index){
        int cmp = array_comparator(entries[j], entries[p_index]);
        if(cmp <= 0){
            tmp = entries[++i];
            entries[i] = entries[j];
            entries[j] = tmp;
        }
        j++;
    }
    /* adds the pivot after */
    tmp = entries[++i];
    entries[i] = entries[p_index];
    entries[p_index] = tmp;
    

    quick_sort_array_aux(entries, s_index, i - 1); /* quick sort on left subarray */

    quick_sort_array_aux(entries, i + 1, p_index); /* quick sort on right subarray */

}   

/*
* Merge sort for array. 
*/
int merge_sort_array(LogArrayManager *log_am){

    /* Invalid Parameters */
    if(log_am == NULL || log_am->count < 2){
        return FAILURE;
    }
    merge_sort_array_aux(log_am, 0, log_am->count-1);
    return SUCCESS;

}

/*
* Auxillary function for merge_sort_array.
*/
static void merge_sort_array_aux(LogArrayManager *log_am, int left, int right){

    int middle = left + (right - left) / 2; /* to avoid overflow */
    if(left >= right){
        return;
    }

    merge_sort_array_aux(log_am, left, middle); /* calls merge sort on the left subarray */

    merge_sort_array_aux(log_am, middle + 1, right); /* calls the merge sort on the right subarray */

    merge(log_am->entries, left, middle, right);

}

/*
* Merges two sorted arrays
*/
static int merge(LogEntry **entries, int left, int middle, int right){
    
    int i = left, j = (middle + 1), k = 0, len = right - left + 1;
    LogEntry **res_arr = calloc(len, sizeof(LogEntry *));
    if(res_arr == NULL){
        return FAILURE;
    }
    /* Fills up res array until eitehr the left or right subarray runs out */
    while(i <= middle && j <= right){
        int cmp = array_comparator(entries[i], entries[j]);
        
        if(cmp <= 0){
            res_arr[k++] = entries[i++];
        }else{
            res_arr[k++] = entries[j++];
        }
    }
    /* left over from the left subarray */
    while(i <= middle){
        res_arr[k++] = entries[i++];
    }
    /* left over from the right subarray */
    while(j <= right){
        res_arr[k++] = entries[j++];
    }

    memmove(entries + left, res_arr, len * sizeof(LogEntry *)); /* copies our result merged array over */
    free(res_arr);

    return SUCCESS;
}

/*
* Heap sort algorithm for our array 
*/
int heap_sort_array(LogArrayManager *log_am){

    /* Invalid Parameters */
    if(log_am == NULL || log_am->count < 2){
        return FAILURE;
    }
    build_max_heap(log_am->entries, log_am->count);
    sort_max_heap(log_am->entries, log_am->count);
    return SUCCESS;

}

/*
* Makes our entry array into a heap. (Builds a max heap)
*/
static void build_max_heap(LogEntry **entries, int len){

    int i = (len - 2) / 2; /* index of first non-leaf node */
    while(i >= 0){
        bubble_down(entries, i--, len);
    }

}

/*
* Bubble down algorithm. (Builds a max heap)
*/
static void bubble_down(LogEntry **entries, int idx, int len){

    int left_child = (2 * idx)  + 1;
    int right_child = (2 * idx) + 2;
    int swapped = 1;

    while(swapped && (left_child < len || right_child < len)){

        LogEntry *tmp;
        int max_child, cmp1;

        /* no left child but yes right child */
        if(left_child >= len && right_child < len){
            max_child = right_child;
        }
        /* yes left child no right child */
        else if(left_child < len && right_child >= len){
            max_child = left_child;
        }
        /* both children present */
        else{
            int cmp2 = array_comparator(entries[left_child], entries[right_child]); /* comparison of left and right child */
            if(cmp2 >= 0){
                max_child = left_child;
            }else{
                max_child = right_child;
            }
        }
        
        cmp1 = array_comparator(entries[idx], entries[max_child]); /* comaprison of parent and max child s*/
        if(cmp1 < 0){
            /* swaps */
            tmp = entries[idx];
            entries[idx] = entries[max_child];
            entries[max_child] = tmp; 
            /* updates fields */
            idx = max_child;   
            left_child = (2 * idx)  + 1;
            right_child = (2 * idx) + 2;
        }else{
            swapped = 0;
        }
        
    }

}

/*
* Sorts an already made max heap.
*/
static void sort_max_heap(LogEntry **entries, int len){
    
    int end_idx = len-1;
    while (end_idx != 0){
        LogEntry *tmp = entries[0];
        entries[0] = entries[end_idx];
        entries[end_idx] = tmp;
        bubble_down(entries, 0, end_idx--);
    }
    
}