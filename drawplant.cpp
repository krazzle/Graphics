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
//#include <openglut.h>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

//extern GLfloat thetaOffset;
extern GLUnurbsObj *theNurb;


std::string Test = "F[+FT]F[-FT]F[=FT]FT";
std::string L0 = "FF[+F2]F[-F1][=F3]F0";
std::string L1 = "F[-F1]F[+F2]F[=FT]F1";
std::string L2 = "F[-FT]F[+F2]F[=F3]F2";
std::string L3 = "F[-F1]F[+FT]F[=F3]F3";

int max_depth = 4;

GLfloat one[] = {1,0,0,0};
GLfloat two[] = {0,1,0,0};
GLfloat three[] = {0,0,1,0};
GLfloat four[] = {0,0,0,1};
GLfloat *mat[] = {one,two,three, four};
unsigned char * treedata;
unsigned int width, height;
static GLuint texName;

GLfloat leafpoints2[4][4][3] = {
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0},  {0, 0, 0}},
{{-1.5, 1, 0}, {-0.5, 1, -1}, {0.5, 1, -1},  {1.5, 1, 0}},
{{-1.5, 2,  0},  {-0.5, 2, -1}, {0.5,2, -1},   {1.5, 2,  0}},
{{0, 3,  0}, {0, 3,  0}, {0, 3, 0},   {0, 3,  0}}};


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

void loadBMP(const char* filepath){

// Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;   // = width*height*3
// Actual RGB data
    //unsigned char * data;
    // Open the file
    FILE* file = fopen(filepath,"rb");
    if (!file)  {printf("Image could not be opened\n"); }
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        //return false;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        //return 0;
    }
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    treedata = new unsigned char [imageSize];

   // Read the actual data from the file into the buffer
    fread(treedata,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

}


void drawLeaf(GLfloat **mat) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	glColor3f(.5,.8,.5); 
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
	other_rand_concav*=2;

	//printf("drawing leaf starting at (%f,%f,%f)\n", x, y, z);
	cpts[0][0] = x; cpts[0][1] = y; cpts[0][2] = z;
	cpts[1][0] = (x+1.5)*random_num; cpts[1][1] = y+1; cpts[1][2] = z;
	cpts[2][0] = x+1.5; cpts[2][1] = y+2;cpts[2][2] = z;
	cpts[3][0] = x; cpts[3][1] = y+3; cpts[3][2]= z;
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
		{{-1.5, 1, 0},              {-0.5, 1, 1-random_concav}, {0.5, 1, 1-random_concav},  {1.5*random_num, 1, 0}},
		{{-1.5*other_rand, 2,  0},  {-0.5, 2, 1-random_concav}, {0.5, 2, 1-random_concav},   {1.5, 2,  0}},
		{{0, 3,  0}, {0, 3,  0}, {0, 3, 0},   {0, 3,  0}}};

	glColor3f(.3,.5,.2);
	int j;
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &leafpoints[0][0][0]);
   	for (j = 0; j <= 8; j++) {
      		glBegin(GL_POLYGON);
      		for (i = 0; i <= 10; i++)
         		glEvalCoord2f((GLfloat)i/10.0, (GLfloat)j/8.0);
      		glEnd();
      		glBegin(GL_POLYGON);
      		for (i = 0; i <= 10; i++)
        		glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/10.0);
      		glEnd();
   	}
   /*	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0,1, 12, 4, &leafpoints2[0][0][0]);
	for (j = 0; j <= 8; j++) {
      		glBegin(GL_POLYGON);
      		for (i = 0; i <= 10; i++)
         		glEvalCoord2f((GLfloat)i/10.0, (GLfloat)j/8.0);
      		glEnd();
      		glBegin(GL_POLYGON);
      		for (i = 0; i <= 10; i++)
         		glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/10.0);
      		glEnd();
   	}*/
	glFlush();
}
void drawBranch(GLfloat percent) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
//	cout << "drawing branch" << endl;	
	
	/*int width, height;
	unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	*/
	//glColor3f(0.54,0.27,0.07);
  	/*GLfloat pi = 3.14159;
	
	int i,j;
	GLfloat treepoints[4][4][3] = 
	{{{1,0,0},{1,7,0},{cos(pi/4),7,sin(pi/4)},{cos(pi/4),0,sin(pi/4)}},
	{{cos((7*pi)/4),0,sin((7*pi)/4)},{cos((7*pi)/4),7,sin((7*pi)/4)},{0,7,-1},{0,0,-1}},
	{{0,0,1},{0,7,1},{cos((3*pi)/4), 7, sin((3*pi)/4)},{cos((3*pi)/4), 0, sin((3*pi)/4)}},
	{{cos((5*pi)/4),0,sin((5*pi)/4)},{cos((5*pi)/4),7,sin((5*pi)/4)},{-1,7,0},{-1,0,0}}};
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &treepoints[0][0][0]);
        for (j = 0; j <= 8; j++) {
                glBegin(GL_POLYGON);
                for (i = 0; i <= 30; i++)
                        glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0);
                glEnd();
                glBegin(GL_POLYGON);
                for (i = 0; i <= 30; i++)
                        glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
                glEnd();
        }*/

	
	//GLUquadricObj*p;
	//p=gluNewQuadric();
	//gluQuadricDrawStyle(p,GLU_LINE);
	//gluCylinder(p,0,1,7,10,10);
	
   //FIBITMAP *currImage = FreeImage_Load(FIF_PNG, "texture_test.png", PNG_DEFAULT);
  // SLD_Surface *currImage = SLD_LoadBMP("texture_test.bmp");
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, currImage);
	glGenTextures(1, &texName);   
 	loadBMP("tree_texture.bmp");
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,
                height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                treedata);
	

	glEnable(GL_TEXTURE_2D);
   	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   	glBindTexture(GL_TEXTURE_2D, texName);


	GLfloat height = 7.0*percent;
  	GLfloat width = 1.0*percent;
  	GLfloat depth = .5*percent;  

  	glColor3f(.5,.3,.2);
	glutCylinder(1.0, 7.0, 15, 15);
//	glutSolidCylinder(1, 7, 15, 15);
	//glutWireTorus(0.1, 0.4, 10, 20);
	//glutCylinder cyl = new GlutCylinder(true, 1.0, 7.0, 15, 10, 5);
	//cyl.position(new Vector(0,0,0), new Vector(0,7,0), 1.0);
	//yl.draw();
	/*glBegin(GL_POLYGON);
	//glTexCoord2f(0,0);
	//glVertex3f(-width, 0, 0);
	//glTexCoord2f(0,1);
	//glVertex3f(-width*.75, height,0);
	//glTexCoord2f(1,0);
	//glVertex3f(width*.75,height,0);
	//glTexCoord2f(1,1);
	//glVertex3f(width,0,0);
 	glVertex3f(width,0.0,-depth);
  	glVertex3f(width*.75,height,-depth*.75);
  	glVertex3f(-width*.75,height,-depth*.75);
  	glVertex3f(-width,0.0,-depth);
  	glVertex3f(width,0.0,depth);
       	glVertex3f(width*.75,height,depth*.75);
        glVertex3f(-width*.75,height,depth*.75);
        glVertex3f(-width,0.0,depth);
   	glEnd();*/
	
	/*glColor3f(.8,.4,.2);
	glBegin(GL_LINE_LOOP);
        glVertex3f(width,0.0,-depth);
        glVertex3f(width*.75,height,-depth*.75);
        glVertex3f(-width*.75,height,-depth*.75);
        glVertex3f(-width,0.0,-depth);
        glVertex3f(width,0.0,depth);
        glVertex3f(width*.75,height,depth*.75);
        glVertex3f(-width*.75,height,depth*.75);
        glVertex3f(-width,0.0,depth);
        glEnd();*/
	
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

void drawLSystem(string str, int depth) {  

//cout << str << endl;
//cout << depth << endl;
    int scale = max_depth - depth + 1;   
    GLfloat percent = 1/((GLfloat)scale);

    int rand_plus = (GLfloat)(rand()%15)+15;
    int rand_minus = (GLfloat)(rand()%15)+15;

    int i;
    GLfloat plus_x = 15;
    GLfloat plus_y = 0;
    GLfloat plus_z = 15;

    
    GLfloat minus_x = 15;
    GLfloat minus_y = 0;
    GLfloat minus_z = -15;
 
    GLfloat equals_x = -15;
    GLfloat equals_y = 0;
    GLfloat equals_z = 0;
      

    if(depth == 0) {
        drawSeed();
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
	    case 'T': drawLeaf(placeholder);
		      break;
	    case '0': if(depth == 1)
		          drawLeaf(placeholder);
 		      else
		          drawLSystem(L0, depth-1);
		      break;
	    case '1': if(depth == 1)
		          drawLeaf(placeholder);
		      else
		          drawLSystem(L1, depth-1);
		      break;
 	    case '2': if(depth == 1)
		          drawLeaf(placeholder);
		      else
		          drawLSystem(L2, depth-1);
		      break;
	    case '3': if(depth == 1)
			  drawLeaf(placeholder);
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


void drawPlant(int depth, GLfloat thetaOffset){
  max_depth = depth;
  int i;
  for ( i = 0; i < 4; i += 1)
    placeholder[i] = new GLfloat[4];

  //printf("thetaoffset: %f\n", thetaOffset);
  copyMatrix(4,4,mat,placeholder);

  placeholder = rotate(placeholder, 0, thetaOffset,0);
  placeholder = translate(placeholder, 1, 0,-25,0);
  //load3DMatrixWrapper(mat);
  drawLSystem(L0, depth);
}

/* end of drawplant.c */
