#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define PATH "shm_file"

void exitFunc(int signal)
{
    printf("\nЗавершение работы приёмника\n");
    exit(0);
}

int main()
{
	setlocale(LC_ALL, "RUS");
	
    signal(SIGINT, exitFunc);

    key_t key = ftok(PATH, 1);
	int shm_id = shmget(key, 32, IPC_EXCL | 0666);
	char* shmat_status = shmat(shm_id, NULL, 0);
	
	if(shm_id == -1)
	{
		err(1);
	}

    if(shmat_status == (char*)(-1))
	{
        err(2);
	}

    while(1)
    {   
		sleep(2);
        time_t timer = time(0);
        printf("\ntime_Consumer = %spid_Consumer = %d\n", ctime(&timer), getpid());
        printf("\n%s", shmat_status);
        
    }
    return 0;
}