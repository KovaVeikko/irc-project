INPUT  = main.c
CFLAGS = -pthread -std=c99 -Wall -g
OUTPUT = main

all: main

main: $(FILES)
	gcc -o $(OUTPUT) $(CFLAGS) $(INPUT)

clean:
	rm -f main
