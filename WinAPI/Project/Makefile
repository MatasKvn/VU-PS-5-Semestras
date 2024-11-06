
OUTPUT = bin/

run: build
	$(OUTPUT)project.exe

build: main.o file1.o file2.o
	g++ $(OUTPUT)main.o $(OUTPUT)file1.o $(OUTPUT)file2.o -o $(OUTPUT)/project.exe

main.o: main.cpp file1.o file2.o
	g++ -c main.cpp -Isrc -o $(OUTPUT)$@

file1.o: src/file1.cpp src/file1.h
	g++ -c src/file1.cpp -o $(OUTPUT)$@

file2.o: src/file2.cpp src/file2.h
	g++ -c src/file2.cpp -o $(OUTPUT)$@
