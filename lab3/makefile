all: compile

clean: 
	rm -rf *.o read write

first: clean
	gcc -o write write.c -Wall
	

second: clean
	gcc -o read read.c -Wall

compile: write read
	