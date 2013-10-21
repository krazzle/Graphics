
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
  GLfloat **deepCopy = new GLfloat*[4];
  int i;
  for ( i = 0 ; i < 4; i += 1)
    deepCopy[i] = new GLfloat[4];
  copyMatrix(4, 4, mat, deepCopy);
  matrixStack.push(deepCopy);
}

GLfloat** pop() {
  /*
  //GLfloat **result = (GLfloat**)malloc(9*sizeof(GLfloat));
  //GLfloat **result = new GLfloat*[3];
  int i;
  for ( i = 0 ; i < 3; i += 1)
    result[i] = new GLfloat[3];
  */
  GLfloat **result = matrixStack.top();
  matrixStack.pop();
  load3DMatrixWrapper(result);
  return result;
}


GLfloat** translate(GLfloat **mat, GLfloat percent, GLfloat x, GLfloat y, GLfloat z) {
  
  //cout << "in translate" << endl;
  
  //translational matrix for a branch with length of 6.0
  GLfloat trans1[4] = {1, 0, 0, x*percent};
  GLfloat trans2[4] = {0, 1, 0, y*percent};
  GLfloat trans3[4] = {0, 0, 1, z*percent};
  GLfloat trans4[4] = {0, 0, 0, 1};
  GLfloat *trans[4] = {trans1, trans2, trans3, trans4};
  
  // i suspect we'll need to generalize for 3d
  int rows = 4;
  int columns = 4;

  //printf("ehhh\n");

  GLfloat** result = multiply(rows, columns, mat, rows, columns, trans);
  
  //cout << "out of multiply" << endl;

  load3DMatrixWrapper(result);	 
  // copy matrices
  copyMatrix(rows, columns, result, mat);

  //cout << "geting out of translate" << endl;
  

  return result;
}

GLfloat** rotate(GLfloat **mat, GLfloat x, GLfloat y, GLfloat z) {
  
  	GLfloat degreesToRads = (2*3.141592654)/180;
	GLfloat rX = x*degreesToRads;
	GLfloat rY = y*degreesToRads;
	GLfloat rZ = z*degreesToRads;

	GLfloat rot1[4] = {cos(rY)*cos(rZ), cos(rY)*sin(rZ), -sin(rY), 0};
	GLfloat rot2[4] = {sin(rX)*sin(rY)*cos(rZ)-cos(rX)*sin(rZ),sin(rX)*sin(rY)*sin(rZ)+cos(rX)*cos(rZ), sin(rX)*cos(rZ), 0};
	GLfloat rot3[4] = {cos(rX)*sin(rY)*cos(rZ) + sin(rX)*sin(rZ), cos(rX)*sin(rY)*sin(rZ)-sin(rX)*cos(rZ), cos(rX)*cos(rY), 0};
	GLfloat rot4[4] = {0,0,0,1};
	GLfloat *rot[4] = {rot1, rot2, rot3, rot4};	
	GLfloat** result = multiply(4, 4, mat, 4, 4 , rot);	  
	load3DMatrixWrapper(result); 
	
	return result;
}



void copyMatrix(int rows, int columns, GLfloat **m1, GLfloat **m2) {

  //printf("m1[%d][%d]: %f\n",2, 0,m1[2][0]);
 // cout << "rows: " << rows << endl;
 // cout << "columns: " << columns << endl;
  
  int i, j;
  for ( i = 0; i < rows; i++) {
    for ( j = 0; j < columns; j++) {
    //  printf("m2[%d][%d]: %f m1[%d][%d]: %f\n",i, j,m2[i][j], i, j, m1[i][j]);
      
      m2[i][j] = m1[i][j];
    }
  }
}

void load2DMatrixWrapper(GLfloat** mat){
	load2DMatrix(mat[0][0],mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]);
}
void load3DMatrixWrapper(GLfloat** mat){
	load3DMatrix(mat[0][0], mat[0][1], mat[0][2], mat[0][3], 
		     mat[1][0], mat[1][1], mat[1][2], mat[1][3], 
		     mat[2][0], mat[2][1], mat[2][2], mat[2][3], 
		     mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}
