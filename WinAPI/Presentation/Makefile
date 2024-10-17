# WinAPI/Presentation/Makefile

CC=g++
CFLAGS=-Wall -Wextra
LDFLAGS=-lopengl32 -lglu32 -lgdi32
IFLAGS=-I../include 
LFLAGS=-L../lib


run: main
	make main
	./main.exe

main: main
	$(CC) $(CFLAGS) -o main.exe main.cpp $(LDFLAGS) $(IFLAGS) $(LFLAGS)


clean:
	rm -f main.exe

do:
	g++ -g --std=c++17 ./main.cpp -lopengl32 -lgdi32 -lglu32 -o main.exe
	./main.exe

other:
	g++ -g --std=c++17 -I./include -L./lib ./other.cpp ./src/glad.c -lglfw3dll -o main.exe
	./main.exe