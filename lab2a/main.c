#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char buf[80];
	const int bufsize = 80;
	setlocale(LC_ALL, "Rus");
	int pipedes[2];
	
	if (pipe(pipedes) == -1) 
	{
        perror("pipe");
        exit(1);
    }
	
	pid_t pid;
	pipe(pipedes);
	switch(pid = fork())
	{
		
		case -1: 				// произошла ошибка
			perror("fork");
			exit(1);
		
		case 0:					// дочерний процесс	
			sleep(5); 			// устанавливает задержку 5 секунд
			
			long int child_buf[2];
			time_t child_time = time(NULL);
			
			close(pipedes[1]); 	// дочерний процесс закрывает выходную часть pipe
			read(pipedes[0], child_buf, sizeof child_buf);
			close(pipedes[0]);
			
			printf("\n CHILD: Время дочернего процесса -- %s", ctime(&child_time));
		
			time_t parent_time = child_buf[0];
			printf(" CHILD: Получил от родителя следующие данные:\n Время родительского процесса %s\n PID: %ld\n", 
			ctime(&child_buf[0]), child_buf[1]);


			exit(0);
		
		default: 				// родительский процесс
			close(pipedes[0]); 	// родительский процесс закрывает входную часть pipe
		
			long int parent_buf[2];
			parent_buf[0] = time(NULL);
			parent_buf[1] = getpid();
			
			printf(" PARENT: Передаю свой PID (%ld) дочернему процессу\n", getpid());
		
			write(pipedes[1], parent_buf, sizeof parent_buf);
			close(pipedes[1]);	
			
			if (waitpid(pid, 0, 0) == -1) 
			{
				perror("waitpid");
				exit(1);
			}
	}
	
	return 0;
}