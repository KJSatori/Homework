CC = g++
CCFLAGS = -g -Wall

proj8: main.o
	$(CC) $(CCFLAGS) -o proj8 main.o

main.o: main.cpp Node.h NodeQueue.h PQ.h
	$(CC) $(CCFLAGS) -c main.cpp

clean:
	rm -f proj8 *.o
