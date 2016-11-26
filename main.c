#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include "structs.h"
#include "backgroundfunctions.h"
#include "sor_multi.h"


void writeFile(double** array, int nx, int ny){
   FILE *f = fopen("./file.txt", "w");
   if (f == NULL) {
      printf("Error opening file.");
      exit(1);
   }
   int i; int j;
   for (i = 0; i < nx; i++) {
      for (j = 0; j < ny; j++) {
         fprintf(f, "%f ", array[i][j]);
      }
      fprintf(f, "\n");
   }
   fclose(f);
}

int main(int argc, char **argv) {
   int nx;
   int ny;
   int cores;
   if (argc > 1) {
      nx = atoi(argv[1]);
      ny = atoi(argv[2]);
      cores = atoi(argv[3]);

   } else {
      nx = 100;
      ny = 100;
   }
   double** V = create2DArray(nx,ny);
   int** boolarr = create2DintArray(nx,ny);

   int i;
   for (i = 1; i < nx - 1; i++) {
      boolarr[i][0] = 1;
      boolarr[i][ny-1] = 1;
   }
   for (i = 0; i < ny; i++) {
      boolarr[0][i] = 1;
      boolarr[nx-1][i] = 1;
   }

   int j;
   for (i = nx/3; i < 2*nx/3; i++) {
      for (j = ny/3; j < 2*ny/3; j++) {
         V[i][j] = 1;
         boolarr[i][j] = 1;
      }
   }

   printf("%s\n", "Grid built, running algorithm...");
   V = sor(V,boolarr,nx,ny,1e-5,cores);
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny);
   free(V);
   free(boolarr);
   return 0;
}
