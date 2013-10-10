
#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <iostream>
#include <stdio.h>

#include "common.h"
#include "drawplant.h"
#include "readppm.h"

#include "helper.h"
#include "l-system-operators.h"

#include <stack>          

using namespace std;

stack<GLfloat**> matrixStack; 

void push(GLfloat **mat) {
  matrixStack.push(mat);
}

GLfloat** pop() {
  GLfloat **result = (GLfloat**)malloc(9*sizeof(GLfloat));
  result = matrixStack.top();
  matrixStack.pop();
  load2DMatrixWrapper(result);
  return result;
}


GLfloat** translate(GLfloat **mat) {
   
  //translational matrix for a branch with length of 6.0
  GLfloat trans1[3] = {1, 0, 0};
  GLfloat trans2[3] = {0, 1, 6.0};
  GLfloat trans3[3] = {0, 0, 1};
  GLfloat *trans[3] = {trans1, trans2, trans3};
  
  // i suspect we'll need to generalize for 3d
  int rows = 3;
  int columns = 3;

  printf("ehhh\n");

  GLfloat** result = multiply(rows, columns, mat, rows, columns, trans);

  load2DMatrix(
	       result[0][0], result[0][1], result[0][2],
	       result[1][0], result[1][1], result[1][2],
	       result[2][0], result[2][1], result[2][2]
	       );	 
  // copy matrices
  copyMatrix(rows, columns, result, mat);
  return result;
}

GLfloat** rotate(GLfloat **mat, GLfloat x, GLfloat y, GLfloat z) {
  
  	GLfloat degreesToRads = (2*3.141592654)/180;
	GLfloat **result;

	if ( y == 0.0 and z == 0.0) {
	  // two dimensional rotation
	  GLfloat rot1[3] = {cos(x*degreesToRads), -sin(x*degreesToRads), 0};
	  GLfloat rot2[3] = {sin(x*degreesToRads), cos(x*degreesToRads), 0};
	  GLfloat rot3[3] = {0, 0, 1};
	  GLfloat *rot[3] = {rot1, rot2, rot3};	
	  result = multiply(3, 3, mat, 3, 3, rot);	  
	  load2DMatrixWrapper(result); 
	}
/*	else {
	  // three dimensional rotation
	  
	  if ( x != 0.0) {
	    GLfloat rot1[4] = {1, 0, 0, 0};
	    GLfloat rot2[4] = {0, cos(x*degreesToRads), -sin(x*degreesToRads), 0};
	    GLfloat rot3[4] = {0, sin(x*degreesToRads), cos(x*degreesToRads), 0};
	    GLfloat rot4[4] = {0, 0, 0, 1};
	    GLfloat *rot[4] = {rot1, rot2, rot3, rot4 };
	    result = multiply(4, 4, mat, 4, 4, rot);
	  }
	  if ( y != 0.0) {
	    GLfloat rot1[4] = {cos(y*degreesToRads), 0, sin(y*degreesToRads), 0};
	    GLfloat rot2[4] = {0, 1, 0, 0};
	    GLfloat rot3[4] = {-sin(y*degreesToRads), 0, cos(y*degreesToRads), 0};
	    GLfloat rot4[4] = {0, 0, 0, 1};
	    GLfloat *rot[4] = {rot1, rot2, rot3, rot4};
	    result = multiply(4, 4, result, 4, 4, rot);
	  }
	  if ( z != 0.0) {
	    GLfloat rot1[4] = {cos(z*degreesToRads), -sin(z*degreesToRads), 0, 0 };
	    GLfloat rot2[4] = {sin(z*degreesToRads), cos(z*degreesToRads), 0, 0};
	    GLfloat rot3[4] = {0, 0, 1, 0};
	    GLfloat rot4[4] = {0, 0, 0, 1};
	    result = multiply(4, 4, result, 4, 4, rot);
	  }
	  
	  load3Dmatrix( 
		       result[0][0], result[0][1], result[0][2], result[0][3],
		       result[1][0], result[1][1], result[1][2], result[1][3],
		       result[2][0], result[2][1], result[2][2], result[2][3],
		       result[3][0], result[3][1], result[3][2], result[3][3],
			);
	}*/ 
	
	copyMatrix(3, 3, result, mat);
	return result;
}



void copyMatrix(int rows, int columns, GLfloat **m1, GLfloat **m2) {

         printf("m1[%d][%d]: %f\n",2, 0,m1[2][0]);
 
  int i, j;
  for ( i = 0; i < rows; i++) {
    for ( j = 0; j < columns; j++) {
      printf("m2[%d][%d]: %f m1[%d][%d]: %f\n",i, j,m2[i][j], i, j, m1[i][j]);

      m2[i][j] = m1[i][j];
    }
  }
}

void load2DMatrixWrapper(GLfloat** mat){
	load2DMatrix(mat[0][0],mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]);
}

