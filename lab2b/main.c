#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH "./fifo_file"

int main()
{
	setlocale(LC_ALL, "Rus");
	int fifodes;
	pid_t pid;
	unlink (PATH);
	mkfifo(PATH, 0777);
	
	switch (pid = fork())
	{

		case -1: // произошла ошибка
		perror("fork");
		exit(1);
	
		case 0: // дочерний процесс
		printf("\n CHILD: Получаю системное время из fifo\n");
		fifodes = open(PATH, O_RDONLY);
		if(fifodes)
		{
			sleep(5);
			
			long int child_buf[2];
			time_t child_time = time(NULL);
			
			read(fifodes, child_buf, sizeof child_buf);
			printf("\n CHILD: Время дочернего процесса -- %s", ctime(&child_time));
            		printf(" CHILD: Время родительского процесса -- %s", ctime(&child_buf[0]));
            		printf(" CHILD: Родительский PID: %ld\n", child_buf[1]);

           		close(fifodes);
            		exit(0);
		}
		
		default: // родительский процесс
		printf("\n PARENT: Помещаю системное время в fifo\n");
		fifodes = open(PATH, O_WRONLY);
		
		if(fifodes)
		{
			long int parent_buf[2];
			parent_buf[0] = time(NULL);
			parent_buf[1] = getpid();

            		write(fifodes, parent_buf, sizeof parent_buf);
			
            		close(fifodes);
		}	
	}
	return 0;
}
