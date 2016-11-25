#include <math.h>
#define PI 3.14159265358979323846

double** sor(double** V, double** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   double R;
   int rc;
   while (Rmax > tol) {
      Rmax = 0;
      //RED Loop
      int i; int j; double R;
      for (i = 1; i < nx; i++) {
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
      //BLACK Loop
      for (i = 1; i < nx; i++) {
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
      N++;
   }
   printf("%d\n", N);
   return V;
}
