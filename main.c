#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include "backgroundfunctions.h"


void writeFile(double* array){
   FILE *f = fopen("./file.txt", "w");
   if (f == NULL) {
      printf("Error opening file.");
      exit(1);
   }
   for (int i; i < length(array); i++) {
      fprintf(f, "%f\n", array[i]);
   }
   fclose(f);
}

char* doubleToChar(double a) {
   static char arr[sizeof(a)];
   memcpy(&arr,&a,sizeof(a));
   return arr;
}

double* foo(int n) {
   double* array = create1DArray(n);
   for (int i = 0; i < n; i++){
      array[i] = i;
   }
   return array;
}

int main(int argc, char **argv) {
   int n;
   if (argc > 1) {
      n = atoi(argv[1]);
   } else {
      n = 1;
   }
   double v_dbl_i[] = {1.0,3.0,-2.0};
   double v_dbl_ii[] = {5.0,-1.2,0.5};
   int vector_length = 3;
   gsl_vector* v_i = gsl_vector_calloc(vector_length);
   gsl_vector* v_ii = gsl_vector_calloc(vector_length);
   for (int i = 0; i < vector_length; i++) {
      gsl_vector_set(v_i,i,v_dbl_i[i]);
      gsl_vector_set(v_ii,i,v_dbl_ii[i]);
   }

   double x;
   gsl_blas_ddot(v_i,v_ii,&x);
   gsl_vector_free(v_i);
   gsl_vector_free(v_ii);
   printf("%f\n", x);
   return 0;
}
