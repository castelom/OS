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

//Global variables
int bytes_write = 0;
int bytes_read  = 0;
int fd[2];

/* The mutex lock */
pthread_mutex_t mutex;

/* the semaphores */
sem_t empty, full;

//Thread ID
pthread_t t_A,t_B;       
pthread_attr_t attr;

//Functions
void *runnerThreadA();/*thread A call this function*/
void *runnerThreadB();/*thread B call this function*/
void initializeData();

//Implementation
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
    sem_post(&full);
    pthread_create(&t_A,&attr,runnerThreadA, NULL); /*create the thread*/
    pthread_create(&t_B,&attr,runnerThreadB, NULL); /*create the thread*/
    
    //Wait threads and finish
    pthread_join(t_A,(void **)NULL);
    pthread_join(t_B,(void **)NULL);
    printf("Finish\n");

}

void *runnerThreadA(){ 
   
   char url[]="1.txt"; // name of my txt
   char buffer[MESSLENGTH];
   int arq = open(url, O_RDONLY);
   int bytes_read_from_file = -1;
   int bytes_write_in_file = 0; 
   if(arq < 0)
   {
      perror("Impossible open the file");
      exit(1);
   }
   
   while(bytes_read_from_file !=0)
   {
      bytes_read_from_file = read(arq, buffer, MESSLENGTH);
      if(bytes_read_from_file !=0){
         bytes_write_in_file = write(fd[1], buffer, bytes_read_from_file);
         bytes_write += bytes_write_in_file;
      }
      if(bytes_read_from_file != 10|0)
         printf("Bytes write = %d\n", bytes_write);
   }
   printf("Finish\n");
   sem_post(&empty);
   close(arq);
}

void *runnerThreadB(){

   char collectFromPipe[MESSLENGTH];
   char last = 'a';
   int receive;
   int bytes = 0;
   int bytes_read = 0;
   int bytes_read_from_file = 0;
   int count = 1;
   receive = open("receive.txt", O_CREAT|O_RDWR, 0666);
   if(receive < 0)
   {
      perror("Impossible to open the receive file");
      exit(1);
   }
   
   sem_wait(&empty);
   
   while(bytes_read != bytes_write){
      
      bytes_read_from_file = read(fd[0], collectFromPipe, MESSLENGTH);
      bytes_read += bytes_read_from_file;
      bytes = write(receive, collectFromPipe, bytes_read_from_file);
      printf("Bytes read for pipe = %d\n",bytes_read);
      printf("Bytes wrote in file = %d\n",bytes);	
      //sem_post(&full);
   }
   close(receive);
}
