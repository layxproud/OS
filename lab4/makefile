all: compile

clean: 
	rm -rf *.o read write

write: clean
	gcc -o write write.c -Wall
	

read: clean
	gcc -o read read.c -Wall

compile: write read
