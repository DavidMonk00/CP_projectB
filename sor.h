#include <math.h>
#define PI 3.14159265358979323846

double** sor(double** V, double** boolarr, int nx, int ny, double tol) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   double R;
   int i;
   while (Rmax > tol) {
      Rmax = 0;
      for (i = 0; i < nx; i++) {
         for (int j = i%2; j < ny; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > Rmax) {
                  Rmax = R;
               }
            }
         }
      }
      for (i = 0; i < nx; i++) {
         for (int j = (i+1)%2; j < ny; j=j+2) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > Rmax) {
                  Rmax = R;
               }
            }
         }
      }
      printf("%f\n", Rmax);
      N++;
   }
   printf("%d\n", N);
   return V;
}
