DEBUG  := yes
CC	   := gcc
CFLAGS := -W -Wall
EXEC   := exec

ifeq ($(DEBUG), yes)
	CFLAGS += -DDEBUG
endif

all: $(EXEC)
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif

exec: main.o sem.o
	$(CC) -o $@ $^ 

main.o: main.c ressources.h
	$(CC) -o $@ -c $< $(CFLAGS)

sem.o: semaphore.c semaphore.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

mrproper: clean
	rm $(EXEC)
