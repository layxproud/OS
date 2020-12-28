#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PATH "./fifo_file"

int main()
{
	setlocale(LC_ALL, "Rus");
	
	char buf[80];
	const int bufsize = 80;
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
		fifodes = open(PATH, O_RDONLY);
		
		if(fifodes)
		{
			sleep(5);
			
			long int child_buf[2];
			time_t child_time = time(NULL);
			
			read(fifodes, buf, bufsize);
			printf("\n CHILD: Время дочернего процесса -- %s", ctime(&child_time));
			printf(" CHILD: Получил от родителя следующие данные:\n%s", buf);

			close(fifodes);
			exit(0);
		}
		
		default: // родительский процесс
		fifodes = open(PATH, O_WRONLY);
		
		if(fifodes)
		{
			long int parent_buf[2];
			time_t parent_time = time(NULL);
			parent_buf[0] = time(NULL);
			parent_buf[1] = getpid();
			
			sprintf(buf, " PARENT: Мой PID -- %d\n Моё время -- %s\n", getpid(), ctime(&parent_time));

			write(fifodes, buf, bufsize);
			
			close(fifodes);
		}
		
		if (waitpid(pid, 0, 0) == -1) 
		{
			perror("waitpid");
			exit(1);
		}		
	}
	return 0;
}