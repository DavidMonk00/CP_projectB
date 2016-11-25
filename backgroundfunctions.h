/* 20161104
 * David Monk
 *
 * This header file contains the matrix operation functions used
 * within the FDM methods implemented for both the single and
 * double pendulums.
 */

#include <gsl/gsl_blas.h>
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
