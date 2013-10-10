/*
 * drawplant.cpp
 * -------------
 * Contains the drawing routine and related helper functions for the
 * L-system fractal plant.  Starter code for Project 2.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <iostream>
#include "helper.h"
#include "common.h"
#include "drawplant.h"
#include "readppm.h"
#include "l-system-operators.h"
#include <stdio.h>
#include <vector>

using namespace std;

/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) {

	GLfloat M3D [16];  /* three dimensional matrix doing same transform */

	M3D[0] = m00;  M3D[1] = m10; M3D[2] = 0.0; M3D[3] = m20;
	M3D[4] = m01;  M3D[5] = m11; M3D[6] = 0.0; M3D[7] = m21;
	M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
	M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

/* Takes a 3D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33) {

	/* ADD YOUR CODE */
}

void drawLeaf(void) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	glColor3f(0.1,0.9,0.1); 
	glBegin(GL_POLYGON);
	glVertex2f(0.0,0.0);
	glVertex2f(1.0,0.7);
	glVertex2f(1.3,1.8);
	glVertex2f(1.0,2.8);
	glVertex2f(0.0,4.0);
	glVertex2f(-1.0,2.8);
	glVertex2f(-1.3,1.8);
	glVertex2f(-1.0,0.7);
	glEnd();
}

void drawBranch(void) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
	glColor3f(0.54,0.27,0.07); 
	glBegin(GL_POLYGON);
	glVertex2f(1.0,0.0);
	glVertex2f(1.0,6.0);
	glVertex2f(-1.0,6.0);
	glVertex2f(-1.0,0.0);
	glEnd();
}

void drawLSystem(char str[], int len){
	
	GLfloat one[] = {1,0,0};
	GLfloat two[] = {0,1,0};
	GLfloat three[] = {0,0,1};
	GLfloat *mat[] = {one,two,three};	
	
	load2DMatrixWrapper((GLfloat**)mat);

	//the reason for placeholder is because translate/rotate return GLfloat** objects and I couldnt cast that to GLfloat *mat[]	
	//if you can come up with a better solution that would be greeeeeeat :D
	GLfloat **placeholder = (GLfloat**)malloc(sizeof(mat));
	printf("before copy\n");
	copyMatrix(3,3,mat,placeholder);

	int i;
	GLfloat left_theta = 30;
	GLfloat right_theta = 330;
	for(i = 0; i < len; i++){
		char temp = str[i];
		switch(temp){
			case 'F':drawBranch(); placeholder = translate(placeholder); 
				printf("drawing branch at: \n");printMatrix((GLfloat**)mat);
				break;
			case '[':push(placeholder); break;
			case ']':placeholder = pop(); break;
			case '-':placeholder = rotate(placeholder, left_theta, 0, 0); break;
			case '+':placeholder = rotate(placeholder, right_theta, 0, 0); break;
			case 'T':drawLeaf();
				printf("drawing leaf at: \n"); printMatrix((GLfloat**)mat);
				break;
			default: break;

		}
	}
}


void printMatrix(GLfloat** mat){
	printf("[%f,%f,%f]\n[%f,%f,%f]\n[%f,%f,%f]\n", mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0],mat[2][1], mat[2][2]);
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawPlant() {
		
	drawLSystem("FT", 2);
	/*
	GLfloat degreesToRads = (2*3.14159)/180;
	GLfloat theta = 0.0;
	GLfloat amplitude = 1.0;
	int i;
	for(i = 0; i < 20; i+=2){
	
		GLfloat frac1 = (GLfloat)((rand() % 10)+1);
		GLfloat frac2 = (GLfloat)((rand() % 5) +1);
		amplitude = frac1/frac2;
		//identity matrix
		GLfloat vec1[3] = {1, 0, 0};
		GLfloat vec2[3] = {0, 1, 0};
		GLfloat vec3[3] = {0, 0, 1};
		GLfloat* mat[3] = {vec1, vec2, vec3};
	
		//translational matrix
		GLfloat trans1[3] = {1, 0, i};
		GLfloat trans2[3] = {0, 1, i};
		GLfloat trans3[3] = {0, 0, 1};
		GLfloat *trans[3] = {trans1, trans2, trans3};
		
		//rotational matrix
		GLfloat rot1[3] = { cos(theta*degreesToRads), -sin(theta*degreesToRads), 0};
		GLfloat rot2[3] = { sin(theta*degreesToRads), cos(theta*degreesToRads), 0};
		GLfloat rot3[3] = { 0, 0, 1};
		GLfloat *rot[3] = {rot1, rot2, rot3};	
		GLfloat** result = multiply(3, 3, trans, 3, 3, rot);
	
	
		load2DMatrix(
			     result[0][0], result[0][1], result[0][2],
			result[1][0], result[1][1], result[1][2],
			result[2][0], result[2][1], result[2][2]
		);	 
	 
		drawLeaf();
		drawBranch();
		theta+=30;
		
	}*/

	

}

/* end of drawplant.c */
