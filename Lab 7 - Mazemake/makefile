CC = g++
CFLAGS = -Wall -g
BIN = Mazemake Mazesolve

all: $(BIN)

Mazemake: Mazemake.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o Mazemake Mazemake.cpp dset.cpp

Mazesolve: Mazesolve.cpp
	$(CC) $(CFLAGS) -o Mazesolve Mazesolve.cpp

clean:
	rm -f $(BIN)
