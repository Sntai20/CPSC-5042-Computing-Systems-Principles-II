// C program to demonstrate working of Semaphores
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

sem_t mutex;
int globalX = 0;
time_t elapsed_time = 0;

void* thread(void* arg)
{

    struct timeval start, end;
    long mtime, seconds, useconds;   

    int val = *(int *) arg;
    printf("Val is %d\n", val);

    printf("\nWaiting..\n");  

    gettimeofday(&start, NULL);

    sem_wait(&mutex);

    //critical section

    gettimeofday(&end, NULL);  

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;   
    elapsed_time+= mtime;

    printf("\nEntered..\n");
    globalX++;
  
    sleep(1);
      
    //signal
    printf("\nJust Exiting...\n");
    sem_post(&mutex);
    pthread_exit(NULL);
    return NULL;
    
}
  
  
int main()
{
    sem_init(&mutex, 0, 1);
    pthread_t t1,t2;

    int val=50;

    pthread_create(&t1,NULL,thread,&val);
    pthread_create(&t2,NULL,thread,&val);

    //  Cleanup as main thread waits for all threads to terminate before it terminates

    pthread_join(t1,NULL);
    sleep(40);
    pthread_join(t2,NULL);
    sem_destroy(&mutex);
    
    printf("GlobalX is %d\n", globalX);
    printf("elapsed waiting time = %ld\n", elapsed_time);
    
    return 0;
}