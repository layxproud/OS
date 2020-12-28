#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#define PATH "shm_file"

int shm_id;

void exitFunc(int sig) // функция завершения работы
{
    printf("\nЗавершение работы передатчика\n");
    struct shmid_ds *buf = 0;
    shmctl(shm_id, IPC_RMID, buf);

    exit(0);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUS");
	
    signal(SIGINT, exitFunc);
	
    key_t key = ftok(PATH, 1);
    shm_id = (shmget(key, 32, IPC_CREAT | 0666));
	char* shmat_status = shmat(shm_id, NULL, 0);

    if(shm_id == -1)
    {
        printf("Can't create shared memory\n");
        exit(0);
    }

    if(shmat_status == (char*)-1)
    {
		printf("Shmat err\n");
		exit(0);
	}
	
	if (strlen(shmat_status) != 0)
    {
        printf("there is already a sending process\n");
        exit(0);
    }
	
	if(argc > 1)
	{
		if(strcmp(argv[1], "-f") == 0)
		{
			 struct shmid_ds *buf = 0;
			 shmctl(shm_id, IPC_RMID, buf);
			 exit(0);
		}
	}

	printf("if you'd like to clear memory, run ./first with -f flag\n");	

    time_t timer = time(0);
    time_t buft = timer;
	
    while(1)
    {
        timer = time(0);
        if (timer != buft)
        {
            buft = timer;
            sprintf(shmat_status, "time_Producer = %spid_Producer = %d\n", ctime(&timer), getpid());
        }
    }
    return 0;
}