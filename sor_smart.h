#include <math.h>
#define PI 3.14159265358979323846

ReturnParams sorSlice(void* initparams) {
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

   double Rmax = 0;
   double R; int i; int j;
   if (red){
      for (i = xStart; i < xEnd; i++) {
         for (int j = i%2; j < yBound; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > Rmax) {
                  Rmax = R;
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
               if (R > Rmax) {
                  Rmax = R;
               }
            }
         }
      }
   }
   ReturnParams ret;
   ret.V = V;
   ret.Rmax = Rmax;
   return ret;
}

double** sor(double** V, int** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   int rc;
   int threads = cores;

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

   while (Rmax > tol) {
      Rmax = 0;
      int i;
      for (i = 0; i < threads; i++) {
         retvals_red[i] = sorSlice((void*)&initparams_red[i]);
      }
      for (i = 0; i < threads; i++) {
         if (retvals_red[i].Rmax > Rmax) {
            Rmax = retvals_red[i].Rmax;
         }
      }
      for (i = 0; i < threads; i++) {
         retvals_black[i] = sorSlice((void*)&initparams_black[i]);
      }
      for (i = 0; i < threads; i++) {

         if (retvals_black[i].Rmax > Rmax) {
            Rmax = retvals_black[i].Rmax;
         }
      }
      N++;
      printf("Rmax after interation %d = %f\n", N, Rmax);
   }
   printf("%d\n", N);
   return V;
}