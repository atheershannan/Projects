CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra
RELEASE = -DNDEBUG -O3
DEBUG = -g
INCLUDE = include -I ../../ds/include
DEPS = include/task.h include/scheduler.h ../../ds/include/uid.h
OBJ =  ../../ds/src/vector.o ../../ds/src/heap.o ../../ds/src/pqueue_heap.o ../../ds/src/uid.o src/task.o src/scheduler_heap.o test/scheduler_test.o src/op_funcs.o
TARGET = scheduler.out


$(TARGET): $(OBJ) $(DEPS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJ)

%.o: %.c $(DEPS)
	$(CC) -c -o $@  $< $(CFLAGS) -I $(INCLUDE)

all: clean
all: $(TARGET)

debug: CFLAGS += $(DEBUG)
debug: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

release: CFLAGS += $(RELEASE)	
release: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	
clean:
	rm -f $(OBJ) *.out


