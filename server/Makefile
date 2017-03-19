FILES  = main.c headers/*.h modules/*.c
INPUT  = main.c modules/*.c
CFLAGS = -pthread -std=c99 -Wall -g
OUTPUT = main

all: main

main: $(FILES)
	gcc -o $(OUTPUT) $(CFLAGS) $(INPUT)

clean:
	rm -f main
