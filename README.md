# log_list_manager
C Log Manager implementing dynamic memory, linked lists, string handling, and file I/O to efficiently create, search, and manage logs.

A simple but powerful **Log Manager** implemented in C following C89 standards.  
It uses **dynamic memory allocation**, **linked lists**, **string manipulation**, and **file I/O** to efficiently create, search, manage, and output log entries.

---

## Features

- Create and manage logs with timestamp, severity, and message fields.
- Dynamic memory management for flexible log storage.
- Linked list structure for efficient insertion and removal.
- Search logs by keyword.
- Remove logs by severity level.
- Write logs to any output stream (console, files, etc.).
- Clear all logs while keeping the manager reusable.

---

## Usage

1. **Create a LogManager:**
    ```c
    LogManager *log_m = create_log_manager();
    ```

2. **Add logs:**
    ```c
    add_log(log_m, "2025-07-06 14:30", "INFO", "Application started");
    ```

3. **Write logs to a stream:**
    ```c
    write_all_logs(log_m, stdout);
    ```

4. **Search logs by keyword:**
    ```c
    search_and_write_logs(log_m, "error", stdout);
    ```

5. **Remove logs by severity:**
    ```c
    remove_logs_by_severity(log_m, "DEBUG");
    ```

6. **Clear all logs:**
    ```c
    clear_logs(log_m);
    ```

7. **Destroy LogManager when done:**
    ```c
    destroy_log_manager(log_m);
    ```

---

## Build Instructions

Compile with:

```bash
gcc -Wall -Wextra -std=c89 -o log_manager sample_driver.c log_list_manager.c
