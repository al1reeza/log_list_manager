# Log Manager in C (Array and Linked List Implementations)

This project provides two separate implementations of a **Log Manager** in the C programming language:

* **Log List Manager** using a **singly linked list**.
* **Log Array Manager** using a **dynamically resizing array**.

Both implementations provide a way to create, store, search, manage, and output log entries consisting of timestamp, severity, and message fields.

---

## Features

* ✅ Create and initialize a log manager (array or list).
* ✅ Dynamically allocate memory for log entries.
* ✅ Add logs with timestamp, severity, and message.
* ✅ Remove logs by severity.
* ✅ Write all logs to a file or console.
* ✅ Clear all logs while keeping the manager reusable.
* ✅ Search logs by keyword (linked list only).
* ✅ Efficient reallocation for array-based storage.

---

## Structures

### Log Entry

```c
typedef struct LogEntry {
    char *time_stamp;
    char *severity;
    char *message;
    struct LogEntry *next; // Used only in Log List Manager
} LogEntry;
```

### Log List Manager

```c
typedef struct {
    LogEntry *head;
    int count;
} LogManager;
```

### Log Array Manager

```c
typedef struct {
    LogEntry **entries;
    size_t count;
    size_t capacity;
} LogArrayManager;
```

---

## Compilation

Use the following commands to compile:

### Linked List Version

```bash
gcc -Wall -Wextra -std=c89 -o log_list sample_driver.c log_list_manager.c
```

### Array Version

```bash
gcc -Wall -Wextra -std=c89 -o log_array sample_driver_array.c log_array_manager.c
```

---

## Usage

### Create a manager

```c
LogManager *log_m = create_log_manager();
LogArrayManager *log_am = create_log_array_manager(10); /* initial capacity */
```

### Add logs

```c
add_log(log_m, "2025-07-26 12:00", "INFO", "Server started");
add_log_entry(log_am, "2025-07-26 12:00", "INFO", "Server started");
```

### Write all logs

```c
write_all_logs(log_m, stdout);
write_all_logs(log_am, stdout);
```

### Search by keyword (Linked List Only)

```c
search_and_write_logs(log_m, "error", stdout);
```

### Remove logs by severity

```c
remove_logs_by_severity(log_m, "DEBUG");
remove_logs_by_severity(log_am, "DEBUG");
```

### Clear logs

```c
clear_logs(log_m);
clear_log_array(log_am);
```

### Destroy manager

```c
destroy_log_manager(log_m);
destroy_log_array_manager(log_am);
```

---

## Sample Drivers

* `sample_driver.c`: Demonstrates usage of the linked list version.
* `sample_driver_array.c`: Demonstrates usage of the array version.

---

## Notes

* All memory is dynamically allocated and properly freed.
* Uses standard C89 style with strict compiler flags.
* All functions return `SUCCESS` or `FAILURE` macros for consistency.
* The array manager automatically doubles capacity when full.
* The list manager supports keyword searching, while the array manager does not.

---

## Future Improvements

* Add sorting functionality.
* Add timestamp parsing and comparison.
* Implement keyword search in array version.
* Add export to JSON or CSV.
