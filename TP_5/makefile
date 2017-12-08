CC       := gcc
CFLAGS   := -std=gnu99 -W -Wall
DEBUG    := no
DFLAGS   :=
EXEC     := reader
SUB_EXEC := file_parser

all: $(EXEC) $(SUB_EXEC)

ifeq ($(DEBUG), yes)
	DFLAGS += -DDEBUG
endif

ifeq ($(DEBUG),yes)
	@echo "Debug release"
else
	@echo "Production release"
endif	


reader: main.o sem.o
	$(CC) -o $@ $^ $(DFLAGS)

file_parser: file_parser.o sem.o
	$(CC) -o $@ $^ $(DFLAGS)

file_parser.o: file_parser.c ressources.h
	$(CC) -o $@ -c $< $(CFLAGS)

main.o: main.c ressources.h
	$(CC) -o $@ -c $< $(CFLAGS) 

sem.o: semaphore.c semaphore.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

mrproper: clean
	rm $(EXEC)
	rm $(SUB_EXEC)
