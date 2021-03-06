 /* -------------
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
//#include <openglut.h>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

#define ORANGE = 0;
#define GREEN = 1;
//extern GLfloat thetaOffset;
extern GLUnurbsObj *theNurb;
int curr_season;

std::string Test = "F[+F?]F[-FT]F[=FT]FT";
std::string L0 = "FF[+F2]F[-F1][=F3]F0";
std::string L1 = "F[-F1]F[+F2]F[=FT]FT";
std::string L2 = "F[-FT]F[+F2]F[=F3]F1";
std::string L3 = "F[-F2]F[+FT]F[=F3]F3";

int max_depth = 4;

GLfloat one[] = {1,0,0,0};
GLfloat two[] = {0,1,0,0};
GLfloat three[] = {0,0,1,0};
GLfloat four[] = {0,0,0,1};
GLfloat *mat[] = {one,two,three, four};
unsigned char * treedata;
unsigned int width, height;
static GLuint texName;

GLfloat branch_vertices[] = {
    -1,  0,  1,
     1,  0,  1,
    -.75, 7,  .75,
     .75, 7,  .75,
    -1,  0, -1,
     1,  0, -1,
    -.75, 7, -.75,
     .75, 7, -.75,
};

GLfloat branch_colors[] = {
    0.645, 0.57, 0.49,
    0.80, 0.69, 0.58,
    0.645, 0.57, 0.49,
    0.80, 0.69, 0.58,
    0.645, 0.57, 0.49,
    0.80, 0.69, 0.58,
    0.645, 0.57, 0.49,
    0.80, 0.69, 0.58,
};

GLuint branch_indices[] = {
    0, 2, 3, 1,
    2, 6, 7, 3,
    7, 6, 4, 5,
    4, 0, 1, 5,
    1, 3, 7, 5,
    0, 4, 6, 2,
};



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

GLfloat** drawFlower(GLfloat **mat){
	push(mat);
	//mat = rotate(mat, 180, 180, 180);
	GLfloat theta = 72;
	int i;
	for(i = 0; i <= 360; i+=theta){
		mat = rotate(mat, 0, 0, theta);
		GLfloat flowerpoints[4][4][3] = {
                	{{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}},
                	{{-1, .75, 0}, {-.5, .75, .5}, {.5, .75, .5},  {1, .75, 0}},
                	{{-1, 1.5,  0},  {-.5, 1.5, .5}, {.5, 1.5, .5},   {1, 1.5,  0}},
                	{{0, 2,  0}, {0, 2,  0}, {0, 2, 0},   {0, 2,  0}}};
		bsplineMap(&flowerpoints[0][0][0], 0, .81, .82, .87, 1.0, 1.0, 10, 15);
	}

	return pop();
}

void drawLeaf(GLfloat **mat, int color) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	glColor3f(.5,.3,0); 
   	GLfloat cpts[7][3];
	int ncpts = 0;

	//render 4 points
	//and 4 points. they share points. 
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;

        GLfloat random_num = (GLfloat)(rand()%2);
        GLfloat other_rand = 1-random_num;

	GLfloat random_concav = (GLfloat)(rand()%2);
	GLfloat other_rand_concav = 1 - random_concav;
	random_concav*=2;
	other_rand_concav*=4;

	//printf("drawing leaf starting at (%f,%f,%f)\n", x, y, z);
	cpts[0][0] = x; cpts[0][1] = y; cpts[0][2] = z;
	cpts[1][0] = (x+1.5)*random_num; cpts[1][1] = y+1; cpts[1][2] = z;
	cpts[2][0] = x+1.5; cpts[2][1] = y+2;cpts[2][2] = z;
	cpts[3][0] = x; cpts[3][1] = y+4; cpts[3][2]= z;
	cpts[4][0] = (x-1.5)*other_rand; cpts[4][1] = y+2; cpts[4][2] = z;
	cpts[5][0] = x-1.5; cpts[5][1] = y+1; cpts[5][2] = z;
	cpts[6][0] = x; cpts[6][1] = y; cpts[6][2] = z;

	ncpts = 7;
    	int i;

  	for(i=0; i<ncpts-3; i +=3)
    	{
        // Draw the curve using OpenGL evaluators 
        	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, cpts[i]);
        	glMapGrid1f(30, 0.0, 1.0);
        	glEvalMesh1(GL_LINE, 0, 30);
    	}
	GLfloat leafpoints[4][4][3] = {
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}},
                {{-1.5, 1, 0},              {-1.5, 2, -.5}, {.5, 2, -.5},  {1.5*random_num, 1, 0}},
                {{-1.5*other_rand, 2,  0},  {-.5, 2, -.5}, {.5, 2, -.5},   {1.5, 2,  0}},
                {{0, 4,  0}, {0, 4,  0}, {0, 4, 0},   {0, 4,  0}}};
	
	if(color == 0)
		bsplineMap(&leafpoints[0][0][0], .82, .41, .11, .96, .64, .37, 10, 15);
	else
		bsplineMap(&leafpoints[0][0][0], .33, .42, .18, .74, .71, .42, 10, 15);
	glFlush();
}

void bsplineMap(GLfloat* data, GLfloat r1, GLfloat g1, GLfloat b1, GLfloat r2, GLfloat g2, GLfloat b2, int i_max, int j_max){
	int j, i;
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, data);
        for (j = 0; j <= j_max; j++) {
                glBegin(GL_POLYGON);
                for (i = 0; i <= i_max; i++){
                        if(i > i_max/2)
                                glColor3f(r1, g1, b1);
                        else
                                glColor3f(r2, g2, b2);
                        glEvalCoord2f((GLfloat)i/i_max, (GLfloat)j/j_max);
                }
                glEnd();
                glBegin(GL_POLYGON);
                for (i = 0; i <= i_max; i++){
                        if(i > i_max/2)
                                glColor3f(r1, g1, b1);
                        else
                                glColor3f(r2, g2, b2);
                        glEvalCoord2f((GLfloat)j/j_max, (GLfloat)i/i_max);
                }
                glEnd();
        }

}

void drawBranch(GLfloat percent) {

  	//glColor3f(.5,.3,.2);
	int num_indices;
	int i;
	int index1,  index2,  index3, index4;
	num_indices = sizeof(branch_indices)/sizeof(GLuint);
	for(i = 0; i < num_indices; i+=4){
		index1 = branch_indices[i]*3;
		index2 = branch_indices[i+1]*3;
		index3 = branch_indices[i+2]*3;
		index4 = branch_indices[i+3]*3;
		glBegin(GL_QUADS);
		glColor3fv(  &(branch_colors[index1]) );
                glVertex3f(branch_vertices[index1]*percent, branch_vertices[index1+1]*percent, branch_vertices[index1+2]*percent);
                glColor3fv(  &(branch_colors[index2]) );
                glVertex3f(branch_vertices[index2]*percent, branch_vertices[index2+1]*percent, branch_vertices[index2+2]*percent);
                glColor3fv(  &(branch_colors[index3]) );
                glVertex3f(branch_vertices[index3]*percent, branch_vertices[index3+1]*percent, branch_vertices[index3+2]*percent);
                glColor3fv(  &(branch_colors[index4]) );
                glVertex3f(branch_vertices[index4]*percent, branch_vertices[index4+1]*percent, branch_vertices[index4+2]*percent);
                glEnd();
	}
}

void drawSeed(GLfloat ** mat) {
  
  	glColor3f(0.54,0.27,0.07);
  	GLfloat seedpoints[4][4][3] = {
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}},
                {{-1.5, 1, 0}, {-1.5, 1, -2}, {.5, 1, -2},  {1.5, 1, 0}},
                {{-1.5, 2,  0},{-.5, 2, -2}, {.5, 2, -2},   {1.5, 2,  0}},
                {{0, 4,  0}, {0, 4,  0}, {0, 4, 0},   {0, 4,  0}}};
  
        GLfloat seedpoints2[4][4][3] = {
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}},
                {{-1.5, 1, 0}, {-1.5, 1, 2}, {.5, 1, 2},  {1.5, 1, 0}},
                {{-1.5, 2,  0},{-.5, 2, 2}, {.5, 2, 2},   {1.5, 2,  0}},
                {{0, 4,  0}, {0, 4,  0}, {0, 4, 0},   {0, 4,  0}}};

	bsplineMap(&seedpoints[0][0][0], .74, .47, .27, .54, .27, .07, 20, 15);
	bsplineMap(&seedpoints2[0][0][0], .74, .47, .27, .54, .27, .07, 20, 15);

        glFlush();

	GLfloat pi = 3.14159;
	load3DMatrix(-cos(pi/4), -sin(pi/4), 0, 15, 
		     -sin(pi/4), cos(pi/4), 0, 2, 
		     0, 0, 1, 0, 
		     0, 0, 0, 1);
	glColor3f(.59, .98, .59);
	glutWireTeapot(7);

}

void drawStar(void) {
  
  glColor3f(1.0, 1.0, 1.0);
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

GLfloat** drawThing(GLfloat** mat){
	if(curr_season == 0){
		drawLeaf(mat, 0);
		return mat;
	}
	else if(curr_season == 1){
		return mat;
	}
	else if(curr_season == 2){
		return drawFlower(mat);
	}
	else{
		drawLeaf(mat, 1);
		return mat;
	}
}

void drawLSystem(string str, int depth) {  

//cout << str << endl;
//cout << depth << endl;
    int scale = max_depth - depth + 1;   
    GLfloat percent = 1/((GLfloat)scale);

    int rand_plus = (GLfloat)(rand()%15)+15;
    int rand_minus = (GLfloat)(rand()%15)+15;

    int i;
    GLfloat plus_x = 17;
    GLfloat plus_y = 2;
    GLfloat plus_z = 13;

    
    GLfloat minus_x = 14;
    GLfloat minus_y = 0;
    GLfloat minus_z = -15;
 
    GLfloat equals_x = -16;
    GLfloat equals_y = 14;
    GLfloat equals_z = 2;
      

    if(depth == 0) {
        drawSeed(placeholder);
        return;
    }

    for(i = 0; i < str.length(); i++){
	char temp = str[i];
	switch(temp){
	    case 'F': drawBranch(percent);
	              placeholder = translate(placeholder, percent, 0, 7, 0);
	              break;
	    case '[': push(placeholder); 
		      break;
	    case ']': placeholder = pop(); 
		      break;
	    case '-': placeholder = rotate(placeholder, minus_x, minus_y, minus_z);
		      break;
	    case '+': placeholder = rotate(placeholder, plus_x, plus_y, plus_z); 
		      break;
  	    case '=': placeholder = rotate(placeholder, equals_x, equals_y, equals_z);
		      break;
	    case 'T': if(curr_season != 2){placeholder = drawThing(placeholder);}
		      break;
	    case '?': placeholder = drawThing(placeholder);
		      break;
	    case '0': if(depth == 1)
		          placeholder = drawThing(placeholder);
 		      else
		          drawLSystem(L0, depth-1);
		      break;
	    case '1': if(depth == 1)
		          placeholder = drawThing(placeholder);
		      else
		          drawLSystem(L1, depth-1);
		      break;
 	    case '2': if(depth == 1)
		          placeholder = drawThing(placeholder);
		      else
		          drawLSystem(L2, depth-1);
		      break;
	    case '3': if(depth == 1)
			  placeholder = drawThing(placeholder);
		      else
			  drawLSystem(L3, depth-1);
	 	      break;
	    default:  break;
	}
    }
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


void drawPlant(int depth, GLfloat thetaOffset, int season){
  max_depth = depth;

  char* seasonStr;

  switch(season) {
  case 0:
    seasonStr = "fall.ppm";
    break;
  case 1:
    seasonStr = "winter.ppm";
    break;
  case 2:
    seasonStr = "spring.ppm";
    break;
  case 3:
    seasonStr = "summer.ppm";
    break;
  default:
    seasonStr = "fall.ppm";
    break;
  }
  load3DMatrix(1, 0, 0, 0,
	       0, 1, 0, 0,
	       0, 0, 1, 0,
	       0, 0, 0, 1
	       );
  int pic_w, pic_h;
  GLubyte* pixels;
  pixels = readPPMfile(seasonStr, &pic_w, &pic_h);
  glRasterPos3f(-40, -40, -5);
  glDrawPixels(pic_w, pic_h, GL_RGB, GL_UNSIGNED_BYTE, pixels); 
  
  curr_season = season;

  int i;
  for ( i = 0; i < 4; i += 1)
    placeholder[i] = new GLfloat[4];

  //printf("thetaoffset: %f\n", thetaOffset);
  copyMatrix(4,4,mat,placeholder);

  placeholder = rotate(placeholder, 0, thetaOffset,0);
  placeholder = translate(placeholder, 1, 0,-32,0);
  
  drawLSystem(L0, depth);
}

/* end of drawplant.c */
