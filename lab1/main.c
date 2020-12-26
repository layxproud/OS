#include <stdio.h>
#include <stdlib.h> // здесь хранится atexit
#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>

void funcDone(void);

int main()
{
	setlocale(LC_ALL, "Rus");
	atexit(funcDone); // обрабатывает указанную функцию на выходе
    pid_t pid;
	switch(pid = fork())
	{
	case -1:
		perror("fork"); // произошла ошибка
		exit(1); // выход из родительского процесса

    case 0:
		printf("\n CHILD: Это процесс-потомок!\n");
		printf(" CHILD: Мой PID -- %d\n", getpid());
		printf(" CHILD: Мой PPID -- %d\n", getppid());
        exit(0);
		
    default:
		printf(" PARENT: Я жду, пока потомок не вызовет exit()...\n");
		wait(NULL);
		printf("\n PARENT: Это процесс-родитель!\n");
		printf(" PARENT: Мой PID -- %d\n", getpid());
		printf(" PARENT: Мой PPID -- %d\n", getppid());
	}
	
	return 0;
}

void funcDone()
{
	printf("atexit завершил работу с PID -- %d\n", getpid());
	return;
}