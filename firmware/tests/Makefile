CC = gcc
CFLAGS = -g
OUTPUT_FILE_NAME := testrunner

OBJS += \
	dcf_parser.o \
	test.o

default: all

all: test

%.o: %.c
	@echo Building file: $< "->" $@
	$(CC) $(CFLAGS) -Wall -c -o "$@" "$<"

$(OUTPUT_FILE_NAME): $(OBJS)
	@echo Linking...
	$(CC) -o $@ $(OBJS)

test: $(OUTPUT_FILE_NAME)
	./$(OUTPUT_FILE_NAME)

clean:
	rm -f $(OUTPUT_FILE_NAME) $(OBJS)
