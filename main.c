#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include "structs.h"
#include "backgroundfunctions.h"
#include "arrayinits.h"
#include "sor_multi.h"


void writeFile(double** array, int nx, int ny, int order) {
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   char s[64];
   strftime(s, sizeof(s), "./data/cable/%Y%m%d%H%M%S", tm);
   char ext[64];
   sprintf(ext, "_%d_%d.lf",nx,order);
   strcat(s,ext);
   //printf("%s\n", s);

   FILE *f = fopen(s, "w");
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
   double tol = 1e-8;
   int order = -8;
   while (mr.N < 10) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny,order);
   free(V);
   free(boolarr);
   return 0;
}
