#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "backgroundfunctions.h"
#include "arrayinits.h"
#include "sor.h"

/* Function: Wire
 * ------------------------
 * Initialises then runs the algorithm for the coaxial
 * cable example
 *
 * tol:     tolerance to which the algorithm will run
 * order:   order of the tolerance
 * nx       number of rows
 * ny:      number of columns
 * cores:   number of cores to use
 * factor:  factor by which to reduce the mesh for initial
 *          seed
 *
 * returns: void
 */
void Wire(double tol, int order, int nx, int ny, int cores,int factor) {
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

/* Function: EDM
 * ------------------------
 * Initialises then runs the algorithm for the electron
 * dipole moment experiemt simulation.
 *
 * tol:     tolerance to which the algorithm will run
 * order:   order of the tolerance
 * scale:   scaling factor for grid
 * cores:   number of cores to use
 * dust:    boolean for the addition of dust to the intial
 *          grid
 *
 * returns: void
 */
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

/* Function: EDMRefining
 * ------------------------
 * Initialises then runs the algorithm for the electron
 * dipole moment experiemt simulation. Extension to original
 * EMD function by creating a coarse seed for the mesh
 *
 * tol:     tolerance to which the algorithm will run
 * order:   order of the tolerance
 * scale:   scaling factor for grid
 * cores:   number of cores to use
 * dust:    boolean for the addition of dust to the intial
 *          grid
 *
 * returns: void
 */
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
   int foo = 1;
   if (argc > 1) {
      if(!strcmp(argv[1], "edm")) {
         foo = 1;
      } else if (!strcmp(argv[1], "cable")){
         foo = 0;
      } else {
         printf("ERROR: Incorrect option selected for mesh type.\nPlease choose either 'edm' or 'cable'\n");
         exit(1);
      }
      scale = atoi(argv[2]);
      tol = atof(argv[3]);
      if (tol >= 1) {
         printf("ERROR: Tolerance must be less than 1.\nRecommended tolerance is 1e-10.\n");
         exit(1);
      }
      if (argc > 4) {
         if (atoi(argv[4])) {
            dust = 1;
         } else {
            dust = 0;
         }
      }
      cores = atoi(argv[5]);
   } else {
      scale = 10;
      tol = 1e-8;
      cores = 4;
   }
   int order = (int)log10(tol);
   printf("Algorithm tolerance: 1e%d\n", order);
   int factor = 4;
   if (foo) {
      EDM(tol,order,scale,cores, dust);
   } else {
      Wire(tol,order,scale,scale,cores,factor);
   }
   return 0;
}
