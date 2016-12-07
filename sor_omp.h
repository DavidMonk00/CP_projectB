#include <math.h>
#include <pthread.h>
#include <omp.h>
#define PI 3.14159265358979323846

double temp_Rmax;

void* sorSlice(void* initparams) {
   InitParams* params = initparams;
   double** V; int** boolarr;
   int xEnd; int xStart; int ny;
   double w; int red;
   V = params->V;
   boolarr = params->boolarr;
   xStart = params->xStart;
   xEnd = params->xEnd;
   ny = params->ny;
   w = params->w;
   red = params->red;
   int yBound = ny - 1;

   temp_Rmax = 0;
   double R; int i; int j;
   if (red){
      for (i = xStart; i < xEnd; i++) {
         #pragma omp parallel for
         for (int j = i%2; j < yBound; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > temp_Rmax) {
                  temp_Rmax = R;
               }
            }
         }
      }
   } else {
      for (i = xStart; i < xEnd; i++) {
         for (j = (i+1)%2; j < yBound; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > temp_Rmax) {
                  temp_Rmax = R;
               }
            }
         }
      }
   }
   //printf("%f\n", temp_Rmax);
   pthread_exit(NULL);
}

MainReturn sor(double** V, int** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   int threads = 1;

   //Thread initialistion
   pthread_t thread[threads];
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   //Parameter initialistion
   InitParams initparams_red[threads];
   InitParams initparams_black[threads];
   ReturnParams retvals_red[threads];
   ReturnParams retvals_black[threads];
   for (long t = 0; t < threads; t++) {
      if (t!=threads-1){
         int rows = nx/threads;
         initparams_red[t].V = V;
         initparams_red[t].boolarr = boolarr;
         initparams_red[t].xStart = t*rows;
         initparams_red[t].xEnd = (t+1)*rows;
         initparams_red[t].ny = ny;
         initparams_red[t].w = w;
         initparams_red[t].red = 1;

         initparams_black[t].V = V;
         initparams_black[t].boolarr = boolarr;
         initparams_black[t].xStart = t*rows;
         initparams_black[t].xEnd = (t+1)*rows;
         initparams_black[t].ny = ny;
         initparams_black[t].w = w;
         initparams_black[t].red = 0;
      } else {
         int rows = nx/threads;
         initparams_red[t].V = V;
         initparams_red[t].boolarr = boolarr;
         initparams_red[t].xStart = t*rows;
         initparams_red[t].xEnd = nx;
         initparams_red[t].ny = ny;
         initparams_red[t].w = w;
         initparams_red[t].red = 1;

         initparams_black[t].V = V;
         initparams_black[t].boolarr = boolarr;
         initparams_black[t].xStart = t*rows;
         initparams_black[t].xEnd = nx;
         initparams_black[t].ny = ny;
         initparams_black[t].w = w;
         initparams_black[t].red = 0;
      }
   }

   //SOR Loop
   int i; void* status;
   while (Rmax > tol) {
      Rmax = 0;
      for (i = 0; i < threads; i++) {
         pthread_create(&thread[i], &attr, sorSlice, (void*)&initparams_red[i]);
         //Rmax_temp[i] = sorSlice((void*)&initparams_red[i]);
         if (temp_Rmax > Rmax) {
            Rmax = temp_Rmax;
         }
      }
      for (i = 0; i < threads; i++) {
         pthread_join(thread[i],&status);
      }
      for (i = 0; i < threads; i++) {
         pthread_create(&thread[i], &attr, sorSlice, (void*)&initparams_black[i]);
         if (temp_Rmax > Rmax) {
            Rmax = temp_Rmax;
         }
         //Rmax_temp[i] = sorSlice((void*)&initparams_black[i]);
      }
      for (i = 0; i < threads; i++) {
         pthread_join(thread[i],&status);
      }
      N++;
      printf("Rmax after interation %d = %0.15lf\n", N, Rmax);
   }
   MainReturn mr;
   mr.N = N;
   mr.V = V;
   return mr;
}
