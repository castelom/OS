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

int main(void){
   int n = -2;
   char collectFromPipe[15];
   char writeinFile[50] = "\nEscreve isso vacilao\nHora da verdade patrao\n";
   arq = open(url, O_RDWR);
 
  // while(n != 0 && n != -1){
   //n = read(arq, collectFromPipe,15);
   //printf("Total bytes = %d, Mess = %s\n", n, collectFromPipe);
   n = write(arq, writeinFile, 25);
   printf("Bytes = %d\n",n);
   printf("lengh = %d\n",(int)strlen(writeinFile));
   //} 
}
