typedef struct {
   double** V;
   int** boolarr;
} Mesh;

typedef struct {
   int N;
   double** V;
} MainReturn;

typedef struct {
   int loops;
   int* starts;
   int* ends;
} LoopParams;

typedef struct {
   //double** V;
   double Rmax;
} ReturnParams;

typedef struct {
   double** V;
   int** boolarr;
   int xStart;
   int xEnd;
   int ny;
   double w;
   int red;
   LoopParams* lp;
} InitParams;
