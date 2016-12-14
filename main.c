#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "backgroundfunctions.h"
#include "arrayinits.h"
#include "sor_multi.h"


void writeFileEDM(double** array, int nx, int ny, int order, int dust) {
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   char s[64];
   strftime(s, sizeof(s), "./data/edm/%Y%m%d%H%M%S", tm);
   char ext[64];
   if (dust) {
      sprintf(ext, "_dust_%d_%d.lf",nx,order);
   } else {
      sprintf(ext, "_%d_%d.lf",nx,order);
   }
   strcat(s,ext);
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

void writeFileWire(double** array, int nx, int ny, int order, int dust) {
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   char s[64];
   strftime(s, sizeof(s), "./data/cable/%Y%m%d%H%M%S", tm);
   char ext[64];
   sprintf(ext, "_%d_%d.lf",nx,order);
   strcat(s,ext);
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
   /*double** V_coarse = generateVArrayWireCoarse(nx/factor,ny/factor);
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

   printf("%s\n", "Grid built, running algorithm...");*/
   double** V = generateVArrayWireCoarse(nx,ny);
   int** boolarr = generateBoolArrayWireCoarse(nx,ny);
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 100) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFileWire(V,nx,ny,order, 0);
   free(V);
   free(boolarr);
}

void EDM(double tol, int order, int scale, int cores, int dust) {
   int nx = 9*scale;
   int ny = 32*scale;
   double** V = generateVArrayEDM(scale,dust);
   printf("Generated array.\n");
   int** boolarr = generateBoolArrayEDM(scale,dust);
   printf("%s\n", "Grid built, running algorithm...");
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 20) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Total iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFileEDM(V,nx,ny,order,dust);
   free(V);
   free(boolarr);
}

void EDMRefining(double tol, int order, int scale, int cores, int dust) {
   int coarse_scale = scale/4;
   int nx_coarse = 9 * coarse_scale;
   int ny_coarse = 32 * coarse_scale;
   double** V_coarse = generateVArrayEDMCoarse(coarse_scale,dust);
   int** boolarr_coarse = generateBoolArrayEDMCoarse(coarse_scale,dust);
   MainReturn mr_coarse;
   mr_coarse.N = 0;
   while (mr_coarse.N < 20) {
      mr_coarse = sor(V_coarse,boolarr_coarse,nx_coarse,ny_coarse,1e-6,cores);
   }
   int nx = 9*scale;
   int ny = 32*scale;
   double** V = generateFineVArray(V_coarse, nx, ny, 4);
   printf("Generated array.\n");
   int** boolarr = generateBoolArrayEDM(scale,dust);
   printf("%s\n", "Grid built, running algorithm...");
   MainReturn mr;
   mr.N = 0;
   while (mr.N < 20) {
      mr = sor(V,boolarr,nx,ny,tol,cores);
      printf("Total iterations: %d\n", mr.N);
   }
   printf("%s\n", "Algorithm complete. Writing to file...");
   writeFileEDM(V,nx,ny,order,dust);
   free(V);
   free(boolarr);
}

int main(int argc, char **argv) {
   double tol;
   int cores;
   int scale;
   int dust;
   if (argc > 1) {
      scale = atoi(argv[1]);
      tol = atof(argv[2]);
      if (atoi(argv[3])) {
         dust = 1;
      } else {
         dust = 0;
      }
      cores = atoi(argv[4]);
   } else {
      scale = 10;
      tol = 1e-8;
      cores = 4;
   }
   int order = (int)log10(tol);
   printf("Algorithm tolerance: 1e%d\n", order);
   int factor = 4;
   Wire(tol,order,scale,scale,cores,factor);
   //EDM(tol,order,scale,cores, dust);
   return 0;
}
