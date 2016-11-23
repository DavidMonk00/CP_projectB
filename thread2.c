#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "backgroundfunctions.h"
#define NUM_THREADS 3

struct thread_data{
   int thread_id;
   double tol;
   double* arr;
};

struct thread_data thread_data_array[NUM_THREADS];

void* sorRow(void* threadarg) {
   double* thread_array;
   thread_array = (double*)threadarg;
   for (int i = 0; i < 10; i++) {
      thread_array[i] = i;
   }
   printf("arr[2]: %f\n", thread_array[2]);
   pthread_exit(thread_array);
}

int main(int argc, char const *argv[]) {
   threads = malloc(NUM_THREADS * sizeof(pthread_t));
   pthread_attr_t attr;
   int rc;

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (long t = 0; t < NUM_THREADS; t++) {

      thread_data_array[t].thread_id = t;
      thread_data_array[t].tol = 0.001;
      double* array = create1DArray(10);

      printf("In main, creating thread %ld\n", t);
      rc = pthread_create(&threads[t], &attr, sorRow, (void*)&array);
      if (rc) {
         printf("Error\n");
         exit(-1);
      }
   }
   for (long t = 0; t < NUM_THREADS; t++) {
      void* retval;
      double* array;
      rc = pthread_join(threads[t], (void*)&retval);
      array = retval;
      printf("In main, returned %f\n", array[2]);
   }
   pthread_exit(NULL);
}
