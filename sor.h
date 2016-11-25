#include <math.h>
#define PI 3.14159265358979323846

struct return_params {
   double** V;
   double Rmax;
};

struct return_params sorSlice(double** V, double** boolarr, int xBound, int yBound, double w, int red) {
   double Rmax = 0;
   double R; int i; int j;
   if (red){
      for (i = 1; i < xBound; i++) {
         for (int j = i%2; j < yBound; j=j+2) {
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
   } else {
      for (i = 1; i < xBound; i++) {
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
   struct return_params ret;
   ret.V = V;
   ret.Rmax = Rmax;
   return ret;
}

double** sor(double** V, double** boolarr, int nx, int ny, double tol, int cores) {
   double t = cos(PI/nx) + cos(PI/ny);
   double w = (8-sqrt(64-16*t*t))/(t*t);
   double Rmax = 1;
   int N = 0;
   int rc;
   int xBound = nx - 1;
   int yBound = ny - 1;
   while (Rmax > tol) {
      Rmax = 0;
      //RED Loop
      struct return_params retvals_red[2];
      retvals_red[0] = sorSlice(slice2DArrayRows(V,ny,0,500),boolarr,500-1,yBound,w,1);
      retvals_red[1] = sorSlice(slice2DArrayRows(V,ny,500,1000),boolarr,500-1,yBound,w,1);
      struct return_params redvals;
      redvals.V = V;
      if(retvals_red[0].Rmax > retvals_red[1].Rmax) {
         redvals.Rmax = retvals_red[0].Rmax;
      } else {
         redvals.Rmax = retvals_red[1].Rmax;
      }
      /*double** V_red = V;
      for (int k = 0; k < 2; k++) {
         V_red = addslice2DArrayRows(V_red, retvals_red[k],ny,k*nx/2,)
      }*/
      //BLACK Loop
      struct return_params retvals_black;
      retvals_black = sorSlice(redvals.V,boolarr,xBound,yBound,w,0);
      if (redvals.Rmax > retvals_black.Rmax) {
         Rmax = redvals.Rmax;
      } else {
         Rmax = retvals_black.Rmax;
      }
      N++;
      V = retvals_black.V;
   }
   printf("%d\n", N);
   return V;
}
