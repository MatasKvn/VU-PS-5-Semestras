# WinAPI/Presentation/Makefile

CC=g++
CFLAGS=-Wall -Wextra
LDFLAGS=-lopengl32 -lgdi32 -lglu32
IFLAGS=-I../include 
LFLAGS=-L../lib


run: main
	make main
	./main.exe

main: main
	$(CC) $(CFLAGS) -o main.exe main.cpp $(LDFLAGS) 


clean:
	rm -f main.exe

do:
	g++ -g --std=c++17 ./main.cpp -lopengl32 -lgdi32 -lglu32 -o main.exe
	./main.exe
