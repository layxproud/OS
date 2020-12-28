#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FILE "shm_file"
#define IPC_ID 1

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUS");
	
	key_t key = ftok(FILE, IPC_ID);
	
	int shmid = shmget(key, 32, IPC_EXCL | 0666);
	if(shmid == -1)
	{
		printf("\nОшибка открытия разделяемой памяти\n");
		exit(-1);
	}	

	char* shmat_status = shmat(shmid, NULL, 0);
	if(shmat_status == (char*) (- 1))
	{
		perror("\nНе могу присоединить разделяемую память\n");
		exit(-1);
	}

	while(1){
		sleep(2);
        time_t timer = time(NULL);
        printf("\n[%d]Приёмник -- %s\n", getpid(), ctime(&timer));
        printf("\nПолученное сообщение:\n%s", shmat_status);
        sleep(1);
	}

	return 0;

}