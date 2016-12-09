#include <math.h>
#include <pthread.h>
#define PI 3.14159265358979323846

double temp_Rmax;

void* sorSlice(void* initparams) {
   InitParams* params = initparams;
   double** V; int** boolarr;
   V = params->V;
   boolarr = params->boolarr;
   int yBound = params->ny - 1;
   temp_Rmax = 0;
   double R; int i; int j;
   if (params->red){
      for (i = params->xStart; i < params->xEnd; i++) {
         for (j = i%2; j < yBound; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + params->w*R;
               if (R > temp_Rmax) {
                  temp_Rmax = R;
               }
            }
         }
      }
   } else {
      for (i = params->xStart; i < params->xEnd; i++) {
         for (j = (i+1)%2; j < yBound; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + params->w*R;
               if (R > temp_Rmax) {
                  temp_Rmax = R;
               }
            }
         }
      }
   }
   pthread_exit(NULL);
}

MainReturn sor(double** V, int** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   int threads = cores*1;

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
   int i; void* status; double res = 0.1;
   float expo = (float)log10(tol);
   clock_t t1,t2;
   t1 = clock();
   while (Rmax > tol) {
      Rmax = 0;
      for (i = 0; i < threads; i++) {
         pthread_create(&thread[i], &attr, sorSlice, (void*)&initparams_red[i]);
      }
      for (i = 0; i < threads; i++) {
         pthread_join(thread[i],&status);
      }
      for (i = 0; i < threads; i++) {
         pthread_create(&thread[i], &attr, sorSlice, (void*)&initparams_black[i]);
      }
      for (i = 0; i < threads; i++) {
         pthread_join(thread[i],&status);
         if (temp_Rmax > Rmax) {
            Rmax = temp_Rmax;
         }
      }
      N++;
      //Progress report
      if (Rmax < res) {
         printf("Max. residual after iteration %d = 1e%d\n", N, (int)log10(Rmax));
         t2 = clock();
         float elapsed = (float)(t2-t1)/(CLOCKS_PER_SEC*cores);
         printf("Time elapsed: %0.1fs | Time remaining: %0.1fs\n", elapsed, (expo*elapsed/(float)log10(Rmax))-elapsed);
         res = res/10;
      }
   }
   MainReturn mr;
   mr.N = N;
   mr.V = V;
   return mr;
}
