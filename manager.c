#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
sem_t *reception_sem;
char STARTING_HOTEL_STATE[30];
int len = 31;
int shm, cmd, mode = 0;
char *addr;
char reception_sem_name[] = "/reception-sem-name"; // semaphore name


void sigfunc(int sig) {
    shm_unlink(SHARED_MEMORY_OBJECT_NAME);

    if(sem_close(reception_sem) == -1) {
        perror("sem_close: Incorrect close of posix semaphore");
        exit(-1);
    }

    if(sem_unlink(reception_sem_name) == -1) {
        perror("sem_unlink: Incorrect unlink of posix semaphore");
        exit(-1);
    }


    printf("bye!!!\n");
    exit (10);
}

int main (int argc, char ** argv) {
    // setting custom process finisher
    signal(SIGINT, sigfunc);
    signal(SIGTERM,sigfunc);

        // all rooms are free
    for (int i = 0; i < len; ++i) {
        STARTING_HOTEL_STATE[i] = '0';
    }

    printf("I am hotel manager, and I am starting a new work day.\n");

printf("before shm_open\n");
    // setting size
    if ( (shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT|O_RDWR, 0777)) == -1 ) {
        perror("shm_open\n");
        return 1;
    }


printf("after shm_open\n");
    // setting size
    // setting size
    if ( ftruncate(shm, len) == -1 ) {
        perror("ftruncate\n");
        return 1;
    }
    printf("after ftruncate\n");

    // getting cursor
    addr = mmap(0, len, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if ( addr == (char*)-1 ) {
        perror("mmap");
        return 1;
    }

    // setting rooms states
    memcpy(addr, STARTING_HOTEL_STATE, len);
    printf("I opened all rooms\n");

    // unsetting cursor
    munmap(addr, len);

printf("unmapped\n");
    // closing shm
    close(shm);

    if((reception_sem = sem_open(reception_sem_name, O_CREAT, 0666, 0)) == 0) {
        perror("sem_open: Can not create posix semaphore\n");
        exit(-1);
    }


    // increasing semaphore
    if(sem_post(reception_sem) == -1) {
        perror("sem_post: Incorrect post of posix semaphore\n");
        exit(-1);
    };
    printf("Reception is opened!\n");


    while (1) {
        sleep(1000);
    }
    return 0;
}
