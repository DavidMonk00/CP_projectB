#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g = 0;

void *myThreadFun(void *arg) {
   int myid = (int)arg;
   static int s = 0;
   ++s; ++g;
   printf("Thread ID: %d, Static: %d, Global: %d\n", myid, ++s, ++g);
   return NULL;
}

int main()
{
   pthread_t tid;
   for (int i = 0; i < 3; i++) {
      pthread_create(&tid, NULL, myThreadFun, (void*)i);
   }
   pthread_exit(NULL);
   exit(0);
}
