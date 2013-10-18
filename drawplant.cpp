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
#include <string>
using namespace std;

//extern GLfloat thetaOffset;

std::string Test = "F[+FT]FT";
std::string L0 = "F[-F1]F[+F2]F0";
std::string L1 = "F[-F1]F[+F2]F1";
std::string L2 = "F[-FT]F[+F2]F2";

int max_depth = 1;

GLfloat one[] = {1,0,0,0};
GLfloat two[] = {0,1,0,0};
GLfloat three[] = {0,0,1,0};
GLfloat four[] = {0,0,0,1};
GLfloat *mat[] = {one,two,three, four};

GLfloat **placeholder = new GLfloat*[4];

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

	GLfloat M3D[16];
	M3D[0] = m00; M3D[1] = m10; M3D[2] = m20; M3D[3] = m30;
	M3D[4] = m01; M3D[5] = m11; M3D[6] = m21; M3D[7] = m31;
	M3D[8] = m02; M3D[9] = m12; M3D[10] = m22; M3D[11] = m32;
	M3D[12] = m03; M3D[13] = m13; M3D[14] = m21; M3D[15] = m33;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

void drawLeaf(void) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	glColor3f(0.1,.5,0.1); 
	glBegin(GL_POLYGON);
	glVertex3f(0,0,0);
	glVertex3f(1.5,.5,0);
	glVertex3f(.5,.3,0);
	glVertex3f(1.8,2.1,0);
	glVertex3f(.4,1.2,0);	
	glVertex3f(0,3,0);
	glVertex3f(-1.5,.5,0);
        glVertex3f(-.5,.3,0);
        glVertex3f(-1.8,2.1,0);
        glVertex3f(-.4,1.2,0);
	glVertex3f(0,0,.1);
        glVertex3f(1.5,.5,.1);
        glVertex3f(.5,.3,.2);
        glVertex3f(1.8,2.1,.2);
        glVertex3f(.4,1.2,.2);
        glVertex3f(0,3,.2);
        glVertex3f(-1.5,.5,.2);
        glVertex3f(-.5,.3,.2);
        glVertex3f(-1.8,2.1,.1);
        glVertex3f(-.4,1.2,0.1);

	glEnd();
	
	glColor3f(.5,1,.5);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0,0,0);
        glVertex3f(1.5,.5,0);
        glVertex3f(.5,.3,0);
        glVertex3f(1.8,2.1,0);
        glVertex3f(.4,1.2,0);
        glVertex3f(0,3,0);
        glVertex3f(-1.5,.5,0);
        glVertex3f(-.5,.3,0);
        glVertex3f(-1.8,2.1,0);
        glVertex3f(-.4,1.2,0);
        glVertex3f(0,0,.1);
        glVertex3f(1.5,.5,.1);
        glVertex3f(.5,.3,.2);
        glVertex3f(1.8,2.1,.2);
        glVertex3f(.4,1.2,.2);
        glVertex3f(0,3,.2);
        glVertex3f(-1.5,.5,.2);
        glVertex3f(-.5,.3,.2);
        glVertex3f(-1.8,2.1,.1);
        glVertex3f(-.4,1.2,0.1);

	glEnd();

}

void drawBranch(GLfloat percent) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
//	cout << "drawing branch" << endl;	
	
	/*int width, height;
	unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	*/
	glColor3f(0.54,0.27,0.07);
  
	GLfloat height = 4.0*percent;
  	GLfloat width = 1.0*percent;
  	GLfloat depth = .5*percent;  

  	glBegin(GL_POLYGON);
  	glVertex3f(width,0.0,-depth);
  	glVertex3f(width,height,-depth);
  	glVertex3f(-width,height,-depth);
  	glVertex3f(-width,0.0,-depth);
  	glVertex3f(width,0.0,depth);
       	glVertex3f(width,height,depth);
        glVertex3f(-width,height,depth);
        glVertex3f(-width,0.0,depth);
   	glEnd();
	
	glColor3f(1,1,0);
	glBegin(GL_LINE_LOOP);
        glVertex3f(width,0.0,-depth);
        glVertex3f(width,height,-depth);
        glVertex3f(-width,height,-depth);
        glVertex3f(-width,0.0,-depth);
        glVertex3f(width,0.0,depth);
        glVertex3f(width,height,depth);
        glVertex3f(-width,height,depth);
        glVertex3f(-width,0.0,depth);
        glEnd();

}

void drawSeed(void) {
  
  glColor3f(0.54,0.27,0.07);
  
  glBegin(GL_POLYGON);
  glVertex3f(1, 0, 0);
  glVertex3f(.7, 1, 0);
  glVertex3f(0, 2, 0);
  glVertex3f(-.7, 1, 0);
  glVertex3f(-1, 0, 0);
  glVertex3f(-.4, -.9, 0);
  glVertex3f(.4, -.9,0);
  glEnd();
  
}

void drawLSystem(string str, int depth, GLfloat thetaOffset) {  

	//cout << str << endl;
	//cout << depth << endl;
	int scale = max_depth - depth + 1;
        GLfloat percent = 1/((GLfloat)scale);

	int i;
	GLfloat left_theta = 19;
	GLfloat right_theta = 341;
      

	if(depth == 0) {
	  drawSeed();
	  return;
	}

	for(i = 0; i < str.length(); i++){
		char temp = str[i];
		switch(temp){
			case 'F':
			  drawBranch(percent);
			  placeholder = translate(placeholder, percent); 
			 // printf("drawing branch at: \n");
			 // printMatrix(placeholder);
			  break;
			case '[':
			  push(placeholder); 
			 // cout << "push push push push" << endl;
			  break;
			case ']':
			  placeholder = pop(); 
			 // cout << "***************pop pop pop pop********" << endl;
			  break;
			case '-':
			  placeholder = rotate(placeholder,0,thetaOffset, left_theta);
			  break;
			case '+':
			  placeholder = rotate(placeholder,0,thetaOffset, right_theta); 
			  break;
			case 'T':
			  drawLeaf();
			 // printf("drawing leaf at: \n"); 
			 // printMatrix(placeholder);
			  break;
			case '0':
			  if(depth == 1)
			    drawLeaf();
 			  else
			    drawLSystem(L0, depth-1,thetaOffset);
			  break;
			case '1':
			  if(depth == 1)
			    drawLeaf();
			  else
			    drawLSystem(L1, depth-1, thetaOffset);
			  break;
 			case '2':
			  if(depth == 1)
			    drawLeaf();
			  else
			    drawLSystem(L2, depth-1, thetaOffset);
			  break;
			default: 
			  break;

		}
	}
}


void rotatePlant(void) {
  
  
  
}


void printMatrix(GLfloat** mat){
	printf("[%f,%f,%f,%f]\n[%f,%f,%f,%f]\n[%f,%f,%f,%f]\n[%f,%f,%f,%f]\n", mat[0][0], mat[0][1], mat[0][2],mat[0][3], mat[1][0], mat[1][1], mat[1][2],mat[1][3], mat[2][0],mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */


void drawPlant(int depth, GLfloat thetaOffset){
  max_depth = depth;
  int i;
  for ( i = 0; i < 4; i += 1)
    placeholder[i] = new GLfloat[4];

  //printf("thetaoffset: %f\n", thetaOffset);
  copyMatrix(4,4,mat,placeholder);

  placeholder = rotate(placeholder, 0, thetaOffset,0);
  //load3DMatrixWrapper(mat);
  drawLSystem(L0, depth, thetaOffset);
}

/* end of drawplant.c */
