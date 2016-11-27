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

int** generateBoolArrayWire(int nx, int ny) {
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
         boolarr[i][j] = 1;
      }
   }
   return boolarr;
}

int** generateBoolArrayWireCoarse(int nx, int ny) {
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
   for (i = nx/3; i < 2*nx/3 + 1; i++) {
      for (j = ny/3; j < 2*ny/3 + 1; j++) {
         boolarr[i][j] = 1;
      }
   }
   return boolarr;
}

double** generateVArrayWire(int nx, int ny) {
   double** V = create2DArray(nx,ny);
   int j; int i;
   for (i = nx/3; i < 2*nx/3; i++) {
      for (j = ny/3; j < 2*ny/3; j++) {
         V[i][j] = 1;
      }
   }
   return V;
}

double** generateVArrayWireCoarse(int nx, int ny) {
   double** V = create2DArray(nx,ny);
   int j; int i;
   for (i = nx/3; i < 2*nx/3 + 1; i++) {
      for (j = ny/3; j < 2*ny/3 + 1; j++) {
         V[i][j] = 1;
      }
   }
   return V;
}



double** generateFineVArray(double** V, int nx, int ny, int factor) {
   double** newV = create2DArray(nx,ny);
   int i; int j;
   for (i = 0; i < nx; i++) {
      for (j = 0; j < ny; j++) {
         newV[i][j] = V[i/factor][j/factor];
      }
   }
   return newV;
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
      cores = 4;
   }

   /*int renorms = 2;
   Mesh meshes[renorms + 1];
   for (int i = 0; i < renorms; i++) {
      int factor = pow(2,renorms - 1);
      meshes[i].V = generateVArrayWireCoarse(nx/factor,ny/factor);
      meshes[i].boolarr = generateBoolArrayWireCoarse(nx/factor,ny/factor);
      MainReturn mr_coarse;
      while (mr_coarse.N < 10) {
         mr_coarse = sor(meshes[i].V,meshes[i].boolarr,nx/factor,ny/factor,1e-5,cores);
         printf("Iterations: %d\n", mr_coarse.N);
      }
   }*/
   int factor = 4;
   double** V_coarse = generateVArrayWireCoarse(nx/factor,ny/factor);
   int** boolarr_coarse = generateBoolArrayWireCoarse(nx/factor,ny/factor);
   MainReturn mr_coarse;
   mr_coarse.N = 0;
   while (mr_coarse.N < 10) {
      mr_coarse = sor(V_coarse,boolarr_coarse,nx/factor,ny/factor,1e-5,cores);
      printf("Iterations: %d\n", mr_coarse.N);
   }
   printf("Coarse array generated.\n");
   double** V = generateFineVArray(V_coarse, nx, ny, factor);
   printf("Generated fine array.\n");
   int** boolarr = generateBoolArrayWire(nx,ny);

   printf("%s\n", "Grid built, running algorithm...");
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 10) {
      mr = sor(V,boolarr,nx,ny,1e-5,cores);
      printf("Iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny);
   free(V);
   free(boolarr);
   return 0;
}
