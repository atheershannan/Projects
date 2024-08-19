CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -pthread -lm
RELEASE = -DNDEBUG -O3
DEBUG = -g
LIBRARY = 
SHARED = ./watchdog.so ./wd_common.so
INCLUDE = ./include -I ../scheduler/include -I ../../ds/include
DEPS = ./include/watchdog.h ../scheduler/include/scheduler.h
OBJ =  ./test/user_test.o
TARGET = user_test.out
SOURCE_FILES := src/watchdog

ifdef FPIC
    LIBRARY += -fpic
	SHARED += -shared
endif

$(TARGET): $(OBJ) $(DEPS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJ) $(SHARED)

%.o: %.c $(DEPS)
	$(CC) -c  -o $@  $< $(CFLAGS) -I $(INCLUDE) $(LIBRARY)

%: %.c
	$(CC) -o $(CURDIR)/$(notdir $@)  $< $(CFLAGS) -I $(INCLUDE)

all: clean
all: $(TARGET)

debug: CFLAGS += $(DEBUG)
debug: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

release: CFLAGS += $(RELEASE)	
release: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	
clean:
	rm -f $(OBJ) $(TARGET).out

