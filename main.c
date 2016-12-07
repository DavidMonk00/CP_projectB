#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "backgroundfunctions.h"
#include "arrayinits.h"
#include "sor.h"


void writeFile(double** array, int nx, int ny, int order) {
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   char s[64];
   strftime(s, sizeof(s), "./data/edm/%Y%m%d%H%M%S", tm);
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
         fprintf(f, "%0.15lf ", array[i][j]);
      }
      fprintf(f, "\n");
   }
   fclose(f);
}

void Wire(double tol, int order, int nx, int ny, int cores,int factor) {
   /*int renorms = 2;
   Mesh meshes[renorms];
   double divisor = pow(factor,renorms);

   meshes[0].V = generateVArrayWireCoarse(nx/divisor,ny/divisor);
   meshes[0].boolarr = generateBoolArrayWireCoarse(nx/divisor,ny/divisor);

   printf("Initial array made.\n");

   for (int i = 1; i < renorms; i++) {
      MainReturn mr_coarse;
      mr_coarse.N = 0;
      divisor = pow(factor,renorms-(i-1));
      printf("Starting iteration %d\n", i);
      while (mr_coarse.N < 10) {
         mr_coarse = sor(meshes[i-1].V,meshes[i-1].boolarr,nx/divisor,ny/divisor,1e-5,cores);
         printf("Iterations: %d\n", mr_coarse.N);
      }

      double divisor = pow(factor,renorms-i);
      printf("%f\n", divisor);
      meshes[i].V = generateFineVArray(meshes[i-1].V,nx/divisor,ny/divisor,factor);
      printf("Generated fine v array\n");
      meshes[i].boolarr = generateBoolArrayWire(nx/divisor,ny/divisor);

      printf("Finished iteration %d\n", i);
   */
   double** V_coarse = generateVArrayWireCoarse(nx/factor,ny/factor);
   int** boolarr_coarse = generateBoolArrayWireCoarse(nx/factor,ny/factor);
   MainReturn mr_coarse;
   mr_coarse.N = 0;
   while (mr_coarse.N < 10) {
      mr_coarse = sor(V_coarse,boolarr_coarse,nx/factor,ny/factor,1e-3,cores);
      printf("Iterations: %d\n", mr_coarse.N);
   }
   printf("Coarse solution found.\n");
   double** V = generateFineVArray(V_coarse, nx, ny, factor);
   printf("Generated fine array.\n");
   int** boolarr = generateBoolArrayWire(nx,ny);

   printf("%s\n", "Grid built, running algorithm...");
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 10) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny,order);
   free(V);
   free(boolarr);
}

void EDM(double tol, int order, int scale, int cores) {
   int nx = 9*scale;
   int ny = 32*scale;
   double** V = generateVArrayEDM(scale);
   printf("Generated array.\n");
   int** boolarr = generateBoolArrayEDM(scale);
   printf("%s\n", "Grid built, running algorithm...");
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 10) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFile(V,nx,ny,order);
   free(V);
   free(boolarr);
}

int main(int argc, char **argv) {
   double tol = 1e-8;
   int order = -8;
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
   int factor = 4;
   int scale = nx;
   //Wire(tol,order,nx,ny,cores,factor);
   EDM(tol,order,scale,cores);
   return 0;
}
