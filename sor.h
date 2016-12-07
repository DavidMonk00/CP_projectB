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
      //#pragma omp parallel for
      for (i = xStart; i < xEnd; i++) {
         for (j = i%2; j < yBound; j=j+2) {
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
      //#pragma omp parallel for
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
   ret.Rmax = Rmax;
   return ret;
}

MainReturn sor(double** V, int** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   int rc;
   int threads = 1;

   InitParams initparams_red;
   InitParams initparams_black;
   ReturnParams retvals_red[threads];
   ReturnParams retvals_black[threads];

   for (long t = 0; t < threads; t++) {
         int rows = nx/threads;
         initparams_red.V = V;
         initparams_red.boolarr = boolarr;
         initparams_red.xStart = t*rows;
         initparams_red.xEnd = nx;
         initparams_red.ny = ny;
         initparams_red.w = w;
         initparams_red.red = 1;

         initparams_black.V = V;
         initparams_black.boolarr = boolarr;
         initparams_black.xStart = t*rows;
         initparams_black.xEnd = nx;
         initparams_black.ny = ny;
         initparams_black.w = w;
         initparams_black.red = 0;
   }

   while (Rmax > tol) {
      Rmax = 0;
      int i;
      for (i = 0; i < threads; i++) {
         retvals_red[i] = sorSlice((void*)&initparams_red);
      }
      for (i = 0; i < threads; i++) {
         if (retvals_red[i].Rmax > Rmax) {
            Rmax = retvals_red[i].Rmax;
         }
      }
      for (i = 0; i < threads; i++) {
         retvals_black[i] = sorSlice((void*)&initparams_black);
      }
      for (i = 0; i < threads; i++) {

         if (retvals_black[i].Rmax > Rmax) {
            Rmax = retvals_black[i].Rmax;
         }
      }
      N++;
      //printf("Rmax after interation %d = %f\n", N, Rmax);
   }
   MainReturn mr;
   mr.N = N;
   mr.V = V;
   return mr;
}
