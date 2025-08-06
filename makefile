CC = gcc
CFLAGS = -Wall -Wextra -std=c89
PROGS = list_driver array_driver

all: $(PROGS)

list_driver: log_list_manager.o sample_driver.o
	$(CC) -o list_driver log_list_manager.o sample_driver.o
log_list_manager.o:	log_list_manager.c log_list_manager.h list_sort_utils.h
	$(CC) $(CFLAGS) -c log_list_manager.c
sample_driver.o: sample_driver.c log_list_manager.h
	$(CC) $(CFLAGS) -c sample_driver.c

array_driver: log_array_manager.o sample_driver_array.o
	$(CC) -o array_driver log_array_manager.o sample_driver_array.o
log_array_manager.o: log_array_manager.c log_array_manager.h
	$(CC) $(CFLAGS) -c log_array_manager.c
sample_driver_array.o: sample_driver_array.c log_array_manager.h
	$(CC) $(CFLAGS) -c sample_driver_array.c

clean:
	rm -f $(PROGS)