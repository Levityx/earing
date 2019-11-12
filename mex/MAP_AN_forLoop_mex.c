/* Based on the c file generated by Matlab
 * Mex file doing the computation to compute IHC_RP, synapticCa, ...
 *
 * Inputs: See list below. matrix_in is the matrix in which the output will be put.
 *  All inputs are double arrays, except for the first that is expected to be int32.
 * Output: None at the moment. Code given below if we want to allocate memory and make an output.
 * Beware: The values of matrix_in and vector_in are changed by the MEX file.
 * Run in Matlab "mex MAP_AN_forLoop.c" to use this file, then MAP_AN_forLoop(int32(.),.,.,.,. )
 *

  case 1: matrix=IHCciliaDisplacement,  vector=uNow,          C double
  case 1: matrix=mICa,                  vector=mICaCurrent0,  C double  
  case 2: matrix=synapticCa,            vector=CaCurrent,     C vector
  case 3: matrix=IHC_RP,                vector=IHC_Vnow,      C matrix

  Main body: loop over columns, loop over rows. 
  for (idx = 0; idx < matrix_size2 ; idx++) {
    for (row = 0; row < matrix_size1; row++){
       // Current index: ind = row + idx * matrix_size1; 
      ind++; 

      // Read release probability (c not defined) 
      c = [ *C  or  C[row]  or  C[ind] ]
      val = vector_in[row] * c + A[ind];

      // Save in the current index 
      vector[row] = val;
      matrix[ind] = [+ or -] val;
    }
  } 
  
*/

#include "mex.h"
#include "matrix.h"

void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
  /* Inputs */
  #define matrix_in prhs[0]
  #define vector_in prhs[1]
  #define C_in prhs[2]
  #define A_in prhs[3]

  /* Variables */
  int ind = 0;        /* Running index */
  int idx;            /* Column index */
  int row;            /* Row index */
  double val;         /* Value to calculate */
  double *C, *A, *vector, *matrix; /* Copy inputs */
  int mat_size1, mat_size2, C_size1, C_size2; /* Size of C and matrix */

  /* Copy original input vectors, get size of C and matrix */
  C         = (double *) mxGetPr(C_in);
  A         = (double *) mxGetPr(A_in);
  vector    = (double *) mxGetPr(vector_in);  
  matrix    = (double *) mxGetPr(matrix_in);
  C_size1   = (int) mxGetM(C_in);
  C_size2   = (int) mxGetN(C_in);
  mat_size1 = (int) mxGetM(matrix_in);
  mat_size2 = (int) mxGetN(matrix_in);

  if (C_size1 == 1 && C_size2 == 1){
    /* printf("Case 1: IHCciliaDisplacement or mICa\n"); */
    for (idx = 0; idx < mat_size2 ; idx++) {
      for (row = 0; row < mat_size1; row++){
        val = vector[row] * *C + A[ind];
        vector[row]   = val;
        matrix[ind++] = val;
      }
    } 
  }

  if (C_size1 != 1 && C_size2 == 1){
    /* printf("Case 2: - synapticCa\n"); */
    for (idx = 0; idx < mat_size2 ; idx++) {
      for (row = 0; row < mat_size1; row++){
        val = vector[row] * C[row] + A[ind];
        vector[row]   = val;
        matrix[ind++] = val;
      }
    }   
  }

  if (C_size1 != 1 && C_size2 != 1){
    /* printf("Case 3: IHC_RP\n"); */
    for (idx = 0; idx < mat_size2 ; idx++) {
      for (row = 0; row < mat_size1; row++){
        val = vector[row] * C[ind] + A[ind];
        vector[row]   = val;
        matrix[ind++] = val;
      }
    } 
  }

  if (C_size1 == 1 && C_size2 != 1){
    printf("C is expected to be a double, a vertical array or matrix, not horizontal.\n");
    abort();
  }
 
  return;
}
