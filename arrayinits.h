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

int** generateBoolArrayEDM(int scale) {
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
   return boolarr;
}

double** generateVArrayEDM(int scale) {
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
   return V;
}
