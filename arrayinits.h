/* 20161210
 * David Monk
 *
 * This header file contains functions used to initialise the meshes used
 * for the simulation.
 */

/* Function: generateBoolArrayWire
 * ------------------------
 * Creates a boolean mesh for the coaxial cable.
 *
 * nx:   number of rows in mesh
 * ny:   number of columns in mesh
 *
 * returns: 2D integer boolean array.
 */
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

/* Function: generateBoolArrayWireCoarse
 * ------------------------
 * Creates a boolean mesh for the coarse precursor to the coaxial
 * cable. This adds an extra fixed column to the right of the cable
 * and an extra row at the bottom. This ensures that there are no errors
 * when refining the result.
 *
 * nx:   number of rows in mesh
 * ny:   number of columns in mesh
 *
 * returns: 2D integer boolean array.
 */
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

/* Function: generateVArrayWire
 * ------------------------
 * Creates a potential mesh for the coaxial cable.
 *
 * nx:   number of rows in mesh
 * ny:   number of columns in mesh
 *
 * returns: 2D double array.
 */
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

/* Function: generateVArrayWire
 * ------------------------
 * Creates a coarse potential mesh for the coaxial cable. Works
 * in the same way as generateBoolArrayWireCoarse.
 *
 * nx:   number of rows in mesh
 * ny:   number of columns in mesh
 *
 * returns: 2D double array.
 */
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

/* Function: generateFineVArray
 * ------------------------
 * Creates a fine potential mesh from initial coarse mesh.
 *
 * V:       original coarse mesh
 * nx:      number of rows in new mesh
 * ny:      number of columns in new mesh
 * factor:  number of new rows/columns per original row/column
 *
 * returns: 2D double array.
 */
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

/* Function: generateBoolArrayEDM
 * ------------------------
 * Creates a boolean mesh for the EDM experiment.
 *
 * scale:   scaling factor for mesh
 * dust:    boolean to add a dust particle to the lower plate
 *
 * returns: 2D integer boolean array.
 */
int** generateBoolArrayEDM(int scale, int dust) {
   int** boolarr = create2DintArray(9*scale,32*scale);
   int i;
   for (i = 1; i < scale*9 - 1; i++) {
      boolarr[i][0] = 1;
      boolarr[i][scale*32-1] = 1;
   }
   for (i = 0; i < scale*32; i++) {
      boolarr[0][i] = 1;
      boolarr[scale*9-1][i] = 1;
   }
   int j;
   for (i = 3*scale; i < 4*scale; i++) {
      for (j = 3*scale; j < 5*scale; j++) {
         boolarr[i][j] = 1;
         boolarr[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale; j++) {
         boolarr[i][j] = 1;
      }
   }
   for (i = 5*scale; i < 6*scale; i++) {
      for (j = 3*scale; j < 5*scale; j++) {
         boolarr[i][j] = 1;
         boolarr[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale; j++) {
         boolarr[i][j] = 1;
      }
   }
   if (dust && scale>99) {
      int size = scale/100;
      for (int i = 0; i < size; i++) {
         for (int j = 0; j < size; j++) {
               boolarr[5*scale - (1+i)][11*scale + j] = 1;
         }
      }
   }
   return boolarr;
}

/* Function: generateVArrayEDM
 * ------------------------
 * Creates a potential mesh for the EDM experiment.
 *
 * scale:   scaling factor for mesh
 * dust:    boolean to add a dust particle to the lower plate
 *
 * returns: 2D double array.
 */
double** generateVArrayEDM(int scale, int dust) {
   double** V = create2DArray(9*scale,32*scale);
   int i; int j;
   for (i = 3*scale; i < 4*scale; i++) {
      for (j = 3*scale; j < 5*scale; j++) {
         V[i][j] = 1;
         V[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale; j++) {
         V[i][j] = 4;
      }
   }
   for (i = 5*scale; i < 6*scale; i++) {
      for (j = 3*scale; j < 5*scale; j++) {
         V[i][j] = -1;
         V[i][j+24*scale] = -1;
      }
      for (j = 6*scale; j < 26*scale; j++) {
         V[i][j] = -4;
      }
   }
   if (dust && scale>99) {
      int size = scale/100;
      for (int i = 0; i < size; i++) {
         for (int j = 0; j < size; j++) {
               V[5*scale - (1+i)][11*scale + j] = -4;
         }
      }
   }
   return V;
}

/* Function: generateBoolArrayEDMCoarse
 * ------------------------
 * Creates a coarse boolean mesh for the EDM experiment. Works in the
 * same way as generateBoolArrayWireCoarse.
 *
 * scale:   scaling factor for mesh
 * dust:    boolean to add a dust particle to the lower plate
 *
 * returns: 2D integer boolean array.
 */
int** generateBoolArrayEDMCoarse(int scale, int dust) {
   int** boolarr = create2DintArray(9*scale,32*scale);
   int i;
   for (i = 1; i < scale*9 - 1; i++) {
      boolarr[i][0] = 1;
      boolarr[i][scale*32-1] = 1;
   }
   for (i = 0; i < scale*32; i++) {
      boolarr[0][i] = 1;
      boolarr[scale*9-1][i] = 1;
   }
   int j;
   for (i = 3*scale; i < 4*scale+1; i++) {
      for (j = 3*scale; j < 5*scale+1; j++) {
         boolarr[i][j] = 1;
         boolarr[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale+1; j++) {
         boolarr[i][j] = 1;
      }
   }
   for (i = 5*scale; i < 6*scale+1; i++) {
      for (j = 3*scale; j < 5*scale+1; j++) {
         boolarr[i][j] = 1;
         boolarr[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale+1; j++) {
         boolarr[i][j] = 1;
      }
   }
   if (dust && scale>99) {
      int size = scale/100;
      for (int i = 0; i < size+1; i++) {
         for (int j = 0; j < size+1; j++) {
               boolarr[5*scale - (1+i)][11*scale + j] = 1;
         }
      }
   }
   return boolarr;
}

/* Function: generateVArrayEDM
 * ------------------------
 * Creates a coarse potential mesh for the EDM experiment. Works
 * in the same way as generateBoolArrayWireCoarse.
 *
 * scale:   scaling factor for mesh
 * dust:    boolean to add a dust particle to the lower plate
 *
 * returns: 2D double array.
 */
double** generateVArrayEDMCoarse(int scale, int dust) {
   double** V = create2DArray(9*scale,32*scale);
   int i; int j;
   for (i = 3*scale; i < 4*scale+1; i++) {
      for (j = 3*scale; j < 5*scale+1; j++) {
         V[i][j] = 1;
         V[i][j+24*scale] = 1;
      }
      for (j = 6*scale; j < 26*scale+1; j++) {
         V[i][j] = 4;
      }
   }
   for (i = 5*scale; i < 6*scale+1; i++) {
      for (j = 3*scale; j < 5*scale+1; j++) {
         V[i][j] = -1;
         V[i][j+24*scale] = -1;
      }
      for (j = 6*scale; j < 26*scale+1; j++) {
         V[i][j] = -4;
      }
   }
   if (dust && scale>99) {
      int size = scale/100;
      for (int i = 0; i < size+1; i++) {
         for (int j = 0; j < size+1; j++) {
               V[5*scale - (1+i)][11*scale + j] = -4;
         }
      }
   }
   return V;
}
