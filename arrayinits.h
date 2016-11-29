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
