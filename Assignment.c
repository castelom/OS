#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MESSLENGTH 10

unsigned int lenght;
char url[]="1.txt"; // name of my txt
int arq;

/* The mutex lock */
pthread_mutex_t mutex;
/* the semaphores */
sem_t empty, full;
//Thread ID
pthread_t t_A,t_B;       
pthread_attr_t attr;

int fd[2];

void *runnerThreadA();/*thread t_consumer this function*/
void *runnerThreadB();/*thread t_producer call this function*/

void initializeData() {
   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Create the 'consumer' semaphore and initialize the value = 0 the subroutine call is blocked*/   
   sem_init(&empty, 0, 0); //

   /* Create the 'producer' semaphore and initialize the value = 0 the subroutine call is blocked */
   sem_init(&full, 0, 0);

   /* Get the default attributes */
   pthread_attr_init(&attr);
}

int main(void){
   
   //arq = open(url, O_RDWR);

 /* Creating pipe */
    if(pipe(fd) < 0)
    {
        perror("pipe error"); /* Using perror() for the first time. */
        exit(1);
    }

    /* Just for interest, print out the fd values. */
    printf("pipe() was successful, fds are %d, %d\n", fd[0], fd[1]);
    // start threads and initialize semaphoros.
    initializeData();      
    pthread_create(&t_A,&attr,runnerThreadA, NULL); /*create the thread*/
    pthread_create(&t_B,&attr,runnerThreadB, NULL); /*create the thread*/
    //close(arq);
    //Wait threads and finish
    pthread_join(t_A,(void **)NULL);
    pthread_join(t_B,(void **)NULL);
    printf("Finish\n");

}

void *runnerThreadA(){

   char buffer[MESSLENGTH];
   arq = open(url, O_RDONLY);
   if(arq < 0)
   {
      perror("Impossible open the file");
      exit(1);
   }
   
   read(arq, buffer, MESSLENGTH);
   write(fd[1], buffer, strlen(buffer));
   sem_post(&empty);
   close(arq);
}

void *runnerThreadB(){

   char collectFromPipe[MESSLENGTH];
   int receive, bytes_read, bytes_write;
   
   receive = open("receive.txt", O_CREAT|O_RDWR, 0666);
   if(arq < 0)
   {
      perror("Impossible to open the receive file");
      exit(1);
   }
   sem_wait(&empty);
   bytes_read = read(fd[0], collectFromPipe, MESSLENGTH);
   bytes_write = write(receive, collectFromPipe, MESSLENGTH);
   printf("Bytes read for pipe = %d\n",bytes_read);
   printf("Bytes wrote in file = %d\n",bytes_write);	
   close(arq);
}
