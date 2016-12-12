/* 20161104
 * David Monk
 *
 * This header file contains basic functions used throughout the code.
 */

#include <math.h>

int length(double* array) {
   return sizeof(array)/sizeof(double);
}

/* Function: create2DArray
 * ------------------------
 * Creates a two-dimensional pointer array in the heap
 *
 * rows:    number of rows in array
 * columns: number of columns in array
 *
 * returns: empty 2D double pointer array.
 */
double** create2DArray(int rows, int columns) {
   double** array;
   array = malloc(rows*sizeof(double*));
   for (int i = 0; i < rows; i++) {
      array[i] = malloc(columns*sizeof(double));
   }
   return array;
}

/* Function: create1DArray
 * ------------------------
 * Creates a one-dimensional pointer array in the heap
 *
 * columns: number of columns in array
 *
 * returns: empty 1D double pointer array.
 */
double* create1DArray(int columns) {
   double* array;
   array = malloc(columns*sizeof(double));
   return array;
}

double** slice2DArrayRows(double** array, int ny, int start_row, int end_row) {
   int rows = end_row - start_row;
   double** slice = create2DArray(rows, ny);
   for (int i = 0; i < rows; i++) {
      slice[i] = array[start_row + i];
   }
   return slice;
}

double** addslice2DArrayRows(double** array, double** slice, int ny, int start_row, int end_row) {
   int rows = end_row - start_row;
   for (int i = 0; i < rows; i++) {
      array[start_row + i] = slice[i + 1];
   }
   return array;
}

/* Function: create2DintArray
 * ------------------------
 * Creates a two-dimensional int pointer array in the heap
 *
 * rows:    number of rows in array
 * columns: number of columns in array
 *
 * returns: empty 2D int pointer array.
 */
int** create2DintArray(int rows, int columns) {
   int** array;
   array = malloc(rows*sizeof(int*));
   for (int i = 0; i < rows; i++) {
      array[i] = malloc(columns*sizeof(int));
   }
   return array;
}

/* Function: create1DintArray
 * ------------------------
 * Creates a one-dimensional int pointer array in the heap
 *
 * columns: number of columns in array
 *
 * returns: empty 1D int pointer array.
 */
int* create1DintArray(int columns) {
   int* array;
   array = malloc(columns*sizeof(int));
   return array;
}

LoopParams* getLoopParams(int** boolarr, int nx, int ny, int breaks) {
   int** loopstarts = create2DintArray(nx, breaks + 1);
   int** loopends = create2DintArray(nx, breaks + 1);
   for (int i = 0; i < nx; i++) {
      int b = 0;
      int breaks = 1;
      for (int j = 0; j < ny; j++) {
         if (boolarr[i][j] == 1 && b == 0) {
            b = 1;
            loopends[i][breaks- 1] = j;
         } else if (boolarr[i][j] == 0 && b == 1) {
            b = 0;
            loopstarts[i][breaks] = j;
            breaks++;
         }
      }
      loopends[i][breaks-1] = ny;
   }
   LoopParams* lp = malloc(nx*sizeof(LoopParams));
   for (int i = 0; i < nx; i++) {
      for (int j = 0; j < breaks + 1; j++) {
         if (!loopends[i][j]) {
            lp[i].loops = j-1;
         } else {
            lp[i].loops = breaks + 1;
         }
      }
      if (lp[i].loops == 0) {
         lp[i].loops++;
      }
   }
   for (int i = 0; i < nx; i++) {
      lp[i].starts = create1DintArray(lp[i].loops);
      lp[i].ends = create1DintArray(lp[i].loops);
      for (int j = 0; j < lp[i].loops; j++) {
         lp[i].starts[j] = loopstarts[i][j];
         lp[i].ends[j] = loopends[i][j];
      }
   }
   return lp;
}
