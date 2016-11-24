#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "backgroundfunctions.h"
#define NUM_THREADS 4

struct thread_data{
   int  thread_id;
   int  nx;
   int ny;
   double* arr;
};

struct thread_data thread_data_array[NUM_THREADS];
struct thread_data return_data_array[NUM_THREADS];

void* sorRow(void* threadarg) {
   struct thread_data* my_data = malloc(sizeof(struct thread_data));
   int taskid;
   int ny;
   double* array;
   my_data = (struct thread_data *) threadarg;
   taskid = my_data->thread_id;
   ny = my_data->ny;
   array = my_data->arr;
   array[2]++;
   printf("arr[2]: %f\n", array[2]);
   printf("Thread: %d\n", taskid);
   pthread_exit((void*)my_data);
}

int main(int argc, char const *argv[]) {
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   int ny = 1000;
   int nx = 1000;
   int i; int j; long t;
   double** base_array = create2DArray(nx,ny);
   for (i = 0; i < nx; i++) {
      for (j = 0; j < ny; j++) {
         base_array[i][j] = i + j;
      }
   }

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (t = 0; t < NUM_THREADS; t++) {

      thread_data_array[t].thread_id = t;
      thread_data_array[t].nx = nx;
      thread_data_array[t].ny = ny;
      thread_data_array[t].arr = base_array[t];

      printf("In main, creating thread %ld\n", t);
      rc = pthread_create(&threads[t], &attr, sorRow, (void*)&thread_data_array[t]);
      if (rc) {
         printf("Error\n");
         exit(-1);
      }
   }
   for (t = 0; t < NUM_THREADS; t++) {
      struct thread_data* res;
      rc = pthread_join(threads[t], (void*)&res);
      double* array;
      array = res->arr;
      base_array[t] = array;
      printf("%f\n", base_array[t][2]);
   }
   pthread_exit(NULL);
}
