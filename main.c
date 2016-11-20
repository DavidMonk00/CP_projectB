#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include "backgroundfunctions.h"
#include "sor.h"


void writeFile(double** array, int nx, int ny){
   FILE *f = fopen("./file.txt", "w");
   if (f == NULL) {
      printf("Error opening file.");
      exit(1);
   }
   for (int i; i < nx; i++) {
      for (int j = 0; j < ny; j++) {
         fprintf(f, "%f ", array[i][j]);
      }
      fprintf(f, "\n");
   }
   fclose(f);
}

char* doubleToChar(double a) {
   static char arr[sizeof(a)];
   memcpy(&arr,&a,sizeof(a));
   return arr;
}

double* foo(int n) {
   double* array = create1DArray(n);
   for (int i = 0; i < n; i++){
      array[i] = i;
   }
   return array;
}

int main(int argc, char **argv) {
   int nx = 1000;
   int ny = 1000;
   double** V = create2DArray(nx,ny);
   double** boolarr = create2DArray(nx,ny);
   for (int i = 1; i < nx - 1; i++) {
      boolarr[i][0] = 1;
      boolarr[i][ny-1] = 1;
   }
   for (int i = 0; i < ny; i++) {
      boolarr[0][i] = 1;
      boolarr[nx-1][i] = 1;
      V[0][i] = 1;
   }
   printf("%s\n", "Grid built, running algorithm...");
   V = sor(V,boolarr,nx,ny,1e-3);
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny);
   free(V);
   return 0;
}
