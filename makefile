CC = gcc
override CFLAGS += -g -Wvla -Wall -Wpedantic -Wextra -Wdeclaration-after-statement

SRCS = $(shell find . -type f -name '*.c')
OBJS = $(patsubst %.c, %.o, $(SRCS))

main: $(OBJS)
	@echo "Compiling program..."
	$(CC) $(CFLAGS) main.c board.c -o main -lm
	@echo "Compilation complete!"

clean:
	@echo "Cleaning environment..."
	rm -f $(OBJS) main
	clear

zip:
	@echo "Zipping files..."
	rm -rf src.zip
	zip -r src.zip *.c *.h board.o
	@echo "Zipping complete!"