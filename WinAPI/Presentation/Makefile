# WinAPI/Presentation/Makefile

CC=g++
CFLAGS=-Wall -Wextra
LDFLAGS=-lopengl32 -lgdi32 
IFLAGS=-I./include 
LFLAGS=-L./lib

g++ -lopengl32 -lgdi32 

run: main
	make main
	./main.exe

main: main
	$(CC) $(CFLAGS) -o main.exe main.cpp $(LDFLAGS) 


clean:
	rm -f main.exe


other:
	g++ -g --std=c++17 -I./external/include ./other.cpp ./external/src/glad.c -lgdi32 -lopengl32 -lglu32 -o main.exe
	./main.exe