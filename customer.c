#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>


#include <sys/mman.h>
#include <signal.h>

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
int len = 31;
int main() {
	printf("hello world!\n");
	
    char sem_name[] = "/reception-sem-name"; // имя семафора
    srand(time(NULL));
    int days = 1 + 20 + (rand() % 7);
    sem_t *p_sem;   // адрес семафора
    printf("I am customer. I woke up. Today i want to go to a hotel for %d days\n", days);
    
    if((p_sem = sem_open(sem_name, O_CREAT, 0666, 0)) == 0) {
        perror("sem_open: Can not create posix semaphore");
        exit(-1);
    }

    int got_a_room = 0;
    int shm;
    char* addr;
    while (!got_a_room) {


    	// Обнуленный семафор ожидает, когда его поднимут, чтобы вычесть 1
    	if(sem_wait(p_sem) == -1) {
        	perror("sem_wait: Incorrect wait of posix semaphore");
        	exit(-1);
    	}
	

	if ( (shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_RDWR, 0777)) == -1 ) {
        	perror("shm_open\n");
        	return 1;
        }

	addr = mmap(0, len, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    	if ( addr == (char*)-1 ) {
        	perror("mmap");
        	return 1;
    	}
	int occupied = addr[len - 1] - '0';
	printf("manager have said that %d rooms are occupied!\n", occupied);
        if (occupied == len - 1) {
		printf("all rooms are occupied!\n");
		if(sem_post(p_sem) == -1) {
                        perror("sem_wait: Incorrect wait of posix semaphore");
                        exit(-1);
                }
	} else {
		got_a_room = 1;
	}





	if (!got_a_room) {
		printf("so, i will go to sleep on a bench near hotel for 5 days\n");
		sleep(5);
	}

    }
   

int room_number = -1;
                addr[len - 1] += 1;
		for (int i = 0; i < len - 1; ++i) {
			if (addr[i] == '0') {
				room_number = i;
				addr[i] = '1';
				break;
			}
		}
               printf("i occupied room %d\n", room_number); 
                if(sem_post(p_sem) == -1) {
                        perror("sem_wait: Incorrect wait of posix semaphore");
                        exit(-1);
                }


                printf("I got a room! Now i go to sleep for %d days\n", days);
                sleep(days);
                printf("I woke up!\n");
		if(sem_wait(p_sem) == -1) {
                	perror("sem_wait: Incorrect wait of posix semaphore");
                	exit(-1);
        	}
		addr[len - 1] -= 1;
		addr[room_number] = '0';
		printf("i gave keys to a manager for a room number %d\n", room_number);


		if(sem_post(p_sem) == -1) {
                        perror("sem_wait: Incorrect wait of posix semaphore");
                        exit(-1);
                }



    printf("Got out from hotel\n");

    if(sem_close(p_sem) == -1) {
        perror("sem_close: Incorrect close of posix semaphore");
        exit(-1);
    }
    printf("Bye!\n");


    return 0;
}

