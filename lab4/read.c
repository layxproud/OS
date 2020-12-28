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
#include <sys/sem.h>

#define SH_FILE "shm_file"
#define IPC_ID 1
#define SEM_FILE "sem_file"
#define SEM_ID 1

struct sembuf semwait = { 0, -1, SEM_UNDO };
struct sembuf semsignal = { 0, 1, SEM_UNDO };

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUS");
	
	key_t key = ftok(SH_FILE, IPC_ID);
	
	int shm_id = shmget(key, 32, IPC_EXCL | 0666);
	if(shm_id == -1)
	{
		printf("\nОшибка открытия разделяемой памяти\n");
		exit(-1);
	}	

	char* shmat_status = shmat(shm_id, NULL, 0);
	if(shmat_status == (char*) (- 1))
	{
		perror("\nНе могу присоединить разделяемую память\n");
		exit(-1);
	}
	
	key_t sem_key = ftok(SEM_FILE, SEM_ID);
	
	int sem_id = semget(sem_key, 1, 0666);
	if(sem_id == -1)
	{
		printf("\nНе могу получить id семафоров\n");
		exit(-1);
	}

	while(1){
		sleep(2);
		
		int sem_signal_res = semop(sem_id, &semsignal, 1);
		if(sem_signal_res == -1)
		{
			printf("\nОшибка с открытием семафоров\n");
			exit(-1);
		}			

        time_t timer = time(NULL);
        printf("\n[%d]Приёмник -- %s\n", getpid(), ctime(&timer));
        printf("Полученное сообщение:\n%s", shmat_status);
		
		int sem_wait_res = semop(sem_id, &semwait, 1);
		if(sem_wait_res == -1)
		{
			printf("\nОшибка с перекрытием семафоров\n");
			exit(-1);
		}
	}

	return 0;

}