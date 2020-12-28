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

int shm_id;

void shared_memory_destructor()
{
	
	if(shmctl(shm_id, IPC_RMID, 0) == -1)
		printf("\nОшибка во время удаления разделяемой памяти\n");
	else
		printf("\nРазделяемая память успешно удалена\n");
}


void function_exit()
{
	struct shmid_ds shminfo;
	shmctl(shm_id, IPC_STAT, &shminfo);
	if(shm_id >= 0 && shminfo.shm_segsz != 0)
		shared_memory_destructor();
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUS");

	if(atexit(function_exit))
	{
		printf("\nATEXIT ERROR\n");
	}

	key_t ipc_key = ftok(SH_FILE, IPC_ID);
	shm_id = shmget(ipc_key, 32, IPC_CREAT | 0666);
	if(shm_id == -1)
	{
		if(errno == EEXIST)
		{
			printf("\nФайл уже существует\n");
			exit(-1);
		}
		else
		{
			perror("\nНе могу получить память\n");
			exit(-1);
		}
	}	

	char* shmat_status = shmat(shm_id, NULL, 0);
	if(shmat_status == (char*)(- 1))
	{
		perror("\nНе могу присоединить разделямую память\n");
		exit(-1);
	}

	if(strlen(shmat_status) != 0)
	{
		printf("\nПриёмник уже запущен\n");
		exit(0);
	}

	key_t sem_key = ftok(SEM_FILE, SEM_ID);
	
	int sem_id = semget(sem_key, 1, 0666 | IPC_CREAT);
	if(sem_id == -1)
	{
		printf("\nНе могу получить id семафоров\n");
		exit(-1);
	}
	
	time_t timer = time(NULL);
    time_t buft = timer;
	
	while(1)
	{
		sleep(2);
		int sem_signal_res = semop(sem_id, &semsignal, 1);
		if(sem_signal_res == -1)
		{
			printf("\nОшибка с открытием семафоров\n");
			exit(-1);
		}

		timer = time(NULL);
		if (timer != buft)
        {
            buft = timer;
            sprintf(shmat_status, "[%d]Передатчик -- %s\n", getpid(), ctime(&timer));
        }
		
		int sem_wait_res = semop(sem_id, &semwait, 1);
		if(sem_wait_res == -1)
		{
			printf("\nОшибка с перекрытием семафоров\n");
			exit(-1);
		}
	}
	
	return 0;

}