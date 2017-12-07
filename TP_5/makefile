DEBUG    := no
CC       := gcc
CFLAGS   := -W -Wall
DFLAGS   :=
EXEC     := reader
SUB_EXEC := file_parser

ifeq ($(DEBUG), yes)
	DFLAGS += -DDEBUG
endif

all: $(EXEC) $(SUB_EXEC)
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif

reader: main.o sem.o
	$(CC) -o $@ $^ 

file_parser: file_parser.o sem.o
	$(CC) -o $@ $^

file_parser.o: file_parser.c ressources.h
	$(CC) -o $@ -c $< $(CFLAGS) $(DFLAGS)

main.o: main.c ressources.h
	$(CC) -o $@ -c $< $(CFLAGS) $(DFLAGS)

sem.o: semaphore.c semaphore.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

mrproper: clean
	rm $(EXEC)
	rm $(SUB_EXEC)
