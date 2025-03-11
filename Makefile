# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: tema3.c
	$(CC) $(CFLAGS) tema3.c -o image_editor -lm

pack:
	zip -FSr 315CA_RotaruFlorisAdrian_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean