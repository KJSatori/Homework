CC = g++
CCFLAGS = -g -w
proj7: main.o BigInt.o
	$(CC) $(CCFLAGS) -o proj7 main.o BigInt.o
main.o: main.cpp BigInt.h
	$(CC) $(CCFLAGS) -c -o main.o main.cpp
BigInt.o: BigInt.h BigInt.cpp
	$(CC) $(CCFLAGS) -c -o BigInt.o BigInt.cpp
clean:
	rm -rf proj7 *.o