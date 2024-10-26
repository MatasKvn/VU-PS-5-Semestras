
run: project
	./project.exe

project: main.o file1.o file2.o
	g++ main.o file1.o file2.o -o project.exe 

main.o: main.cpp file1.o file2.o
	g++ -c main.cpp -Isrc

file1.o: src/file1.cpp src/file1.h
	g++ -c src/file1.cpp

file2.o: src/file2.cpp src/file2.h
	g++ -c src/file2.cpp