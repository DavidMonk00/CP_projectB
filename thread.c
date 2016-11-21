#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define NUM_THREADS	4

struct arg_struct {
   int arg1;
   double* arr;
};

void *BusyWork(void *arguments)
{
   struct arg_struct *args = arguments;
   long tid;
   double result=0.0;
   tid = (long)args -> arg1;
   double* array = args -> arr;
   printf("Thread %ld starting...\n",tid);
   for (int i=0; i<10000000; i++)
   {
      result = result + cos((double)i) * tan((double)i);
   }
   printf("Thread %ld done. Result = %e\n",tid, result);
   pthread_exit((void*)arguments);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   void *status;
   double arr[] = {0.0,0.0,0.0,0.0};

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0; t<NUM_THREADS; t++) {
      struct arg_struct args;
      args.arg1 = t;
      args.arr = arr;
      printf("Main: creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, BusyWork, (void *)&args);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
      }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for(t=0; t<NUM_THREADS; t++) {
      rc = pthread_join(thread[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      }

printf("Main: program completed. Exiting.\n");
pthread_exit(NULL);
}
