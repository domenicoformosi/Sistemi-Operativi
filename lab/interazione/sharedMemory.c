#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>


#define NUM_ITERATIONS 1000

int main() {
    int shmid;
    key_t key = 1234;
    int *shared_counter;
    sem_t *sem;
    
    /*
        The shmget function in Unix is used to allocate a shared memory segment.
        It stands for "shared memory get" and is part of the System V IPC
        (Inter-Process Communication) mechanisms
        key: A unique identifier for the shared memory segment. This can be generated using the ftok function or can be a predefined constant.
        size: The size of the shared memory segment in bytes.
        shmflg: Flags that control the creation and permissions of the shared memory segment. Common flags include:
        IPC_CREAT: Create the segment if it does not exist.
        IPC_EXCL: Fail if the segment already exists.
        0666: Permissions for the segment (read and write for everyone).
    */
    // Create shared memory segment
    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment
    shared_counter = (int *)shmat(shmid, NULL, 0);
    if (shared_counter == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    *shared_counter = 0; // Initialize shared counter

    // Create a semaphore
    sem = sem_open("/sem", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Fork two child processes
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) {
        // Child process 1
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            sem_wait(sem); // Wait for semaphore
            (*shared_counter)++; // Increment shared counter
            sem_post(sem); // Signal semaphore
        }
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) {
        // Child process 2
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            sem_wait(sem); // Wait for semaphore
            (*shared_counter)++; // Increment shared counter
            sem_post(sem); // Signal semaphore
        }
        exit(0);
    }

    // Parent process waits for both children to finish
    wait(NULL);
    wait(NULL);

    printf("Final counter value: %d\n", *shared_counter);

    // Detach and remove shared memory segment
    shmdt(shared_counter);
    shmctl(shmid, IPC_RMID, NULL);

    // Close and unlink the semaphore
    sem_close(sem);
    sem_unlink("/sem");

    return 0;
}