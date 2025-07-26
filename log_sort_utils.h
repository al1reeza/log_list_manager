#ifndef LOG_SORT_UTILS_H
#define LOG_SORT_UTILS_H

#include "log_list_manager.h"    // For linked list log structures
#include "log_array_manager.h"   // For array log structures

/**
 * A comparator function pointer type for sorting LogEntry elements.
 * Should return:
 *   - < 0 if a < b
 *   - 0 if a == b
 *   - > 0 if a > b
 */
typedef int (*LogComparator)(const LogEntry *a, const LogEntry *b);

/* ===================== Array Sorting ===================== */

/**
 * Sorts an array of LogEntry pointers in-place using the given comparator.
 *
 * @param logs         Array of pointers to LogEntry.
 * @param log_count    Number of logs in the array.
 * @param comparator   Function to compare two LogEntry values.
 */
void sort_log_array(LogEntry **logs, int log_count, LogComparator comparator);

/* ===================== Linked List Sorting ===================== */

/**
 * Sorts a linked list of LogEntry nodes in-place using merge sort and the comparator.
 *
 * @param head         Pointer to the head pointer of the linked list.
 * @param comparator   Function to compare two LogEntry values.
 */
void sort_log_list(LogEntry **head, LogComparator comparator);

#endif // LOG_SORTING_UTILS_H
