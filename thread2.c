#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "backgroundfunctions.h"
#define NUM_THREADS 4

struct thread_data{
   int  thread_id;
   int  rows;
   int ny;
   double** arr;
};

struct thread_data thread_data_array[NUM_THREADS];

void* sorRow(void* threadarg) {
   struct thread_data* data = malloc(sizeof(struct thread_data));
   int taskid;
   int rows;
   double** array;
   data = (struct thread_data *) threadarg;
   taskid = data->thread_id;
   rows = data->rows;
   array = data->arr;
   array[0][2]++;

   pthread_exit((void*)data);
}

int main(int argc, char const *argv[]) {
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   int ny = 10000;
   int nx = 10000;
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

      thread_data_array[t].ny = ny;
      if (t != NUM_THREADS-1){
         int rows = nx/NUM_THREADS;
         printf("Thread %ld, rows in chunk: %d\n", t+1, rows);
         thread_data_array[t].rows = rows;
         thread_data_array[t].arr = create2DArray(rows,ny);
      } else {
         int rows = nx/NUM_THREADS + nx%NUM_THREADS;
         printf("Thread %ld, rows in chunk: %d\n", t+1, rows);
         thread_data_array[t].rows = rows;
         thread_data_array[t].arr = create2DArray(rows,ny);
      }
      rc = pthread_create(&threads[t], &attr, sorRow, (void*)&thread_data_array[t]);
      if (rc) {
         printf("Error\n");
         exit(-1);
      }
   }
   for (t = 0; t < NUM_THREADS; t++) {
      struct thread_data* res;
      rc = pthread_join(threads[t], (void*)&res);
      double** array;
      array = res->arr;
      //base_array[t] = array;
      printf("%f\n", array[0][2]);
   }
   pthread_exit(NULL);
}
