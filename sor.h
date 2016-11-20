#include <math.h>
#define PI 3.14159265358979323846

double** sor(double** V, double** boolarr, int nx, int ny, double tol) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   double R;
   while (Rmax > tol) {
      Rmax = 0;
      for (int i = 0; i < nx; i++) {
         for (int j = 0; j < ny; j++) {
            if (!boolarr[i][j]) {
               R = (V[i-1][j]+V[i+1][j]+V[i][j-1]+V[i][j+1])/4 - V[i][j];
               V[i][j] = V[i][j] + w*R;
               if (R > Rmax) {
                  Rmax = R;
               }
            }
         }
      }
      N++;
   }
   return V;
}
