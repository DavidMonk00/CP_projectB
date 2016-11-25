#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#define PI 3.14159265358979323846

struct thread_data{
   int  thread_id;
   int  rows;
   int ny;
   double** V;
   double** boolarr;
   double w;
   double Rmax;
};

struct return_params {
   double** V;
   double Rmax;
};

double** slice2DArrayRows(double** array, int ny, int start_row, int end_row) {
   int rows = end_row - start_row;
   double** slice = create2DArray(rows, ny);
   for (int i = 0; i < rows; i++) {
      slice[i] = array[start_row + i];
   }
   return slice;
}

double** addslice2DArrayRows(double** array, double** slice, int ny, int start_row, int end_row) {
   int rows = end_row - start_row;
   for (int i = 0; i < rows; i++) {
      array[start_row + i] = slice[i];
   }
   return array;
}

void* sorRowRed(void* threadarg) {
   struct thread_data* chunk = malloc(sizeof(struct thread_data));
   chunk = (struct thread_data *) threadarg;
   int taskid = chunk->thread_id;
   int rows = chunk->rows;
   int ny = chunk->ny;
   double** V = chunk->V;
   double** boolarr = chunk->boolarr;
   double Rmax = chunk->Rmax;
   double w = chunk->w;

   //printf("%s\n", "Variables initialised, starting loop...");
   int i; int j; double R;
   for (i = 1; i < rows - 1; i++) {
      for (int j = i%2; j < ny; j=j+2) {
         //printf("i=%d, j=%d\n", i,j);
         if (!boolarr[i][j]) {
            //printf("%s\n", "Passed bool check");
            R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
            //printf("%s\n", "Calculated residual");
            V[i][j] = V[i][j] + w*R;
            if (R > Rmax) {
               Rmax = R;
            }
         }
      }
   }
   chunk->Rmax = Rmax;
   //printf("Rmax_red after loop = %f\n", Rmax);
   pthread_exit((void*)chunk);
}

void* sorRowBlack(void* threadarg) {
   struct thread_data* chunk = malloc(sizeof(struct thread_data));
   chunk = (struct thread_data *) threadarg;
   int taskid = chunk->thread_id;
   int rows = chunk->rows;
   int ny = chunk->ny;
   double** V = chunk->V;
   double** boolarr = chunk->boolarr;
   double Rmax = chunk->Rmax;
   double w = chunk->w;

   //printf("%s\n", "Variables initialised, starting loop...");
   int i; int j; double R;
   for (i = 1; i < rows - 1; i++) {
      for (j = (i+1)%2; j < ny; j=j+2) {
         if (!boolarr[i][j]) {
            R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
            V[i][j] = V[i][j] + w*R;
            if (R > Rmax) {
               Rmax = R;
            }
         }
      }
   }
   chunk->Rmax = Rmax;
   //printf("Rmax_black after loop = %f\n", Rmax);
   pthread_exit((void*)chunk);
}

struct return_params sorIter(double** V, double** boolarr, int nx, int ny, int cores, int red){
   int NUM_THREADS = cores;
   struct thread_data thread_data_array[NUM_THREADS];
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (t = 0; t < NUM_THREADS; t++) {
      thread_data_array[t].thread_id = t;
      thread_data_array[t].w = 1.9;
      thread_data_array[t].ny = ny;
      thread_data_array[t].Rmax = 0;
      if (t == 0){
         int rows = nx/NUM_THREADS;
         thread_data_array[t].rows = rows;
         thread_data_array[t].V = slice2DArrayRows(V, ny, 0, rows+1);
         thread_data_array[t].boolarr = slice2DArrayRows(V, ny, 0, rows+1);
      } else if (t = NUM_THREADS-1){
         int rows = nx/NUM_THREADS + nx%NUM_THREADS;
         thread_data_array[t].rows = rows;
         thread_data_array[t].V = slice2DArrayRows(V, ny, nx - rows - 1, nx);
         thread_data_array[t].boolarr = slice2DArrayRows(V, ny, nx - rows - 1, nx);
      } else {
         int rows = nx/NUM_THREADS;
         thread_data_array[t].rows = rows;
         thread_data_array[t].V = slice2DArrayRows(V, ny, t*rows - 1, (t + 1)*rows+1);
         thread_data_array[t].boolarr = slice2DArrayRows(V, ny, t*rows - 1, (t + 1)*rows+1);
      }
      if (red == 1) {
         rc = pthread_create(&threads[t], &attr, sorRowRed, (void*)&thread_data_array[t]);
      } else {
         rc = pthread_create(&threads[t], &attr, sorRowBlack, (void*)&thread_data_array[t]);
      }
      if (rc) {
         printf("Error\n");
         exit(-1);
      }
   }

   struct return_params retvals;
   retvals.Rmax = 0;
   for (t = 0; t < NUM_THREADS; t++) {
      struct thread_data* res;
      rc = pthread_join(threads[t], (void*)&res);
      int start_row = res->thread_id * res->rows + 1;
      int end_row = start_row + res->rows;
      retvals.V = addslice2DArrayRows(V,res->V,ny,start_row,end_row);
      if (res->Rmax > retvals.Rmax) {
         retvals.Rmax = res->Rmax;
      }
   }
   return retvals;
}

double** sor(double** V, double** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   double R;
   int rc;
   for (int k = 0; k < 4; k++) {
   //while (Rmax > tol) {
      Rmax = 0;
      //RED Loop
      struct return_params retvals_red;
      retvals_red = sorIter(V,boolarr,nx,ny,cores,1);
      //BLACK Loop
      struct return_params retvals_black;
      retvals_black = sorIter(retvals_red.V,boolarr,nx,ny,cores,0);
      N++;
      if (retvals_red.Rmax > retvals_black.Rmax) {
         Rmax = retvals_red.Rmax;
      } else {
         Rmax = retvals_black.Rmax;
      }
      printf("Rmax after iteration %d = %f\n",N, Rmax);
      V = retvals_black.V;
   }
   printf("%d\n", N);
   return V;
}
