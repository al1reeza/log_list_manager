# Log Manager

A comprehensive C library for managing log entries with multiple data structure implementations and sorting algorithms. This project demonstrates efficient memory management, data structure operations, and algorithmic implementations for log management systems.

## Overview

The Log Manager project provides two distinct implementations for handling log entries:
- **Array-based implementation** (`Log_array_manager`) - Dynamic array with automatic resizing
- **Linked list implementation** (`Log_list_manager`) - Linked list structure

Both implementations support common operations like adding, removing, searching, and sorting log entries, making it easy to choose the most appropriate data structure for your use case.

## Features

### Core Functionality
- ✅ **Dynamic Memory Management** - Automatic resizing and efficient memory allocation
- ✅ **Multiple Data Structures** - Choose between array-based or linked list implementations
- ✅ **Comprehensive Sorting** - Quick sort, merge sort, heap sort, and bubble sort algorithms
- ✅ **Flexible Operations** - Add, remove, search, and clear log entries
- ✅ **File I/O Support** - Write logs to files or stdout
- ✅ **Memory Safety** - Proper cleanup and error handling

### Log Entry Structure
Each log entry contains:
- **Timestamp** - When the event occurred
- **Severity Level** - Severity of the event.
- **Message** - Descriptive text about the event

### Sorting Algorithms

#### Array Implementation
- **Quick Sort** - O(n log n) average case, in-place sorting
- **Merge Sort** - O(n log n) guaranteed, stable sorting
- **Heap Sort** - O(n log n) guaranteed, in-place sorting

#### Linked List Implementation  
- **Bubble Sort** - O(n²) worst case, simple implementation
- **Merge Sort** - O(n log n) guaranteed, optimized for linked lists

## Project Structure

```
log-manager/
├── src/
│   ├── log_array_manager.c      # Array implementation
│   ├── log_list_manager.c       # Linked list implementation
│   ├── sample_driver.c          # List demo program
│   └── sample_driver_array.c    # Array demo program
├── include/
│   ├── log_array_manager.h      # Array header file
│   └── log_list_manager.h       # List header file
├── Makefile                     # Build configuration
└── README.md                    # This file
```

## Getting Started

### Prerequisites
- GCC compiler with C89/C90 support
- Make utility
- Unix-like environment (Linux, macOS, WSL)

### Building the Project

Build the project:

```bash
make all
```

This will create two executable programs:
- `list_driver` - Demonstrates linked list implementation
- `array_driver` - Demonstrates array implementation

### Running the Demos

Test the linked list implementation:
```bash
./list_driver
```

Test the array implementation:
```bash
./array_driver
```

### Clean Build Files
```bash
make clean
```

## Usage Examples

### Array Implementation

```c
#include "log_array_manager.h"

LogArrayManager *manager = create_log_array_manager(10);

add_log_entry(manager, "2025-01-15 10:30:00", "INFO", "System started");
add_log_entry(manager, "2025-01-15 10:31:00", "WARN", "Low memory");
add_log_entry(manager, "2025-01-15 10:32:00", "ERROR", "Connection failed");

merge_sort_array(manager);

write_all_logs(manager, stdout);

remove_logs_by_severity(manager, "WARN");

destroy_log_array_manager(manager);
```

### Linked List Implementation

```c
#include "log_list_manager.h"

LogManager *manager = create_log_manager();

add_log(manager, "2025-01-15 10:30:00", "INFO", "Application started");
add_log(manager, "2025-01-15 10:31:00", "DEBUG", "Processing request");

merge_sort_logs(manager);

search_and_write_logs(manager, "request", stdout);

destroy_log_manager(manager);
```

## API Reference

### Array Manager Functions

| Function | Description | Return Value |
|----------|-------------|--------------|
| `create_log_array_manager(size_t capacity)` | Creates new array manager | Pointer to manager or NULL |
| `destroy_log_array_manager(LogArrayManager *mgr)` | Frees all memory | SUCCESS/FAILURE |
| `add_log_entry(manager, timestamp, severity, message)` | Adds new log entry | SUCCESS/FAILURE |
| `remove_logs_by_severity(manager, severity)` | Removes matching logs | Number removed |
| `quick_sort_array(LogArrayManager *mgr)` | Sorts using quicksort | SUCCESS/FAILURE |
| `merge_sort_array(LogArrayManager *mgr)` | Sorts using mergesort | SUCCESS/FAILURE |
| `heap_sort_array(LogArrayManager *mgr)` | Sorts using heapsort | SUCCESS/FAILURE |
| `write_all_logs(manager, stream)` | Outputs all logs | SUCCESS/FAILURE |
| `clear_log_array(LogArrayManager *mgr)` | Removes all entries | SUCCESS/FAILURE |

### List Manager Functions

| Function | Description | Return Value |
|----------|-------------|--------------|
| `create_log_manager()` | Creates new list manager | Pointer to manager or NULL |
| `destroy_log_manager(LogManager *mgr)` | Frees all memory | SUCCESS/FAILURE |
| `add_log(manager, timestamp, severity, message)` | Adds new log entry | SUCCESS/FAILURE |
| `remove_logs_by_severity(manager, severity)` | Removes matching logs | Number removed |
| `bubble_sort_logs(LogManager *mgr)` | Sorts using bubble sort | SUCCESS/FAILURE |
| `merge_sort_logs(LogManager *mgr)` | Sorts using merge sort | SUCCESS/FAILURE |
| `search_and_write_logs(manager, keyword, stream)` | Search and output logs | Number found |
| `write_all_logs(manager, stream)` | Outputs all logs | SUCCESS/FAILURE |
| `clear_logs(LogManager *mgr)` | Removes all entries | SUCCESS/FAILURE |

## Performance Characteristics

### Time Complexity

| Operation | Array Implementation | List Implementation |
|-----------|---------------------|-------------------|
| Add Entry | O(1) amortized | O(1) |
| Remove by Severity | O(n) | O(n) |
| Quick Sort | O(n log n) avg | N/A |
| Merge Sort | O(n log n) | O(n log n) |
| Heap Sort | O(n log n) | N/A |
| Bubble Sort | N/A | O(n²) |
| Search | O(n) | O(n) |

### Space Complexity
- **Array Implementation**: O(n) with dynamic resizing
- **List Implementation**: O(n) with per-node allocation

## Error Handling

The library uses consistent error handling:
- `SUCCESS` (0) for successful operations
- `FAILURE` (-1) for errors
- Return counts for operations that remove/find entries
- NULL pointers for failed memory allocations

All functions validate input parameters and handle edge cases gracefully.

## Memory Management

The library implements robust memory management:
- **Dynamic allocation** for all strings and structures
- **Automatic cleanup** when destroying managers
- **Reallocation** for array growth (doubles capacity)
- **Memory leak prevention** with proper cleanup functions

## Code Quality Features

- **C89/C90 Compliance** - Compatible with older C standards
- **Static Functions** - Proper encapsulation of internal helpers
- **Consistent Naming** - Clear, descriptive function names
- **Error Checking** - Comprehensive input validation
- **Documentation** - Well-commented code
- **Modular Design** - Separate headers and implementations

## Comparison: Array vs Linked List

### Choose Array Implementation When:
- You need **fast random access** to log entries
- **Memory efficiency** is important (less overhead per entry)
- You want **better cache performance**
- **Sorting performance** is critical

### Choose Linked List Implementation When:
- You frequently **insert/delete** entries
- **Memory allocation** needs to be flexible
- You want to **search by keywords** in messages
- **Dynamic sizing** without reallocation is preferred

## Contributing

Contributions are welcome! Please ensure:
- Code follows C89/C90 standards
- All functions include proper error checking
- Memory management is handled correctly
- New features include appropriate test cases

## License

MIT License
Copyright (c) 2025 Alireza Alvandi
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
