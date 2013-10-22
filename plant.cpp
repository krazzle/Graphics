 
/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "mouse.h"
#include "helper.h"
#include "l-system-operators.h"
#include "common.h"
#include "drawplant.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */
int depth = 4;
GLfloat  thetaOffset = -40;
GLUnurbsObj *theNurb;

/* local function declarations */
void display(void);
void init(void);
void myKeyHandler(unsigned char ch, int x, int y);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseButton(myMouseButton);
  glutMouseMotion(myMouseMotion);
  glEnable(GL_MAP1_VERTEX_3);
  glutMainLoop();
  return 0;
}

void init() {
   //GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
   //GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   //GLfloat mat_shininess[] = { 100 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   //glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);
  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_AUTO_NORMAL);
  // glEnable(GL_NORMALIZE);
  glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);


	   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   //glGenTextures(1, &texName);
   //glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);

   //FIBITMAP *currImage = FreeImage_Load(FIF_PNG, "texture_test.png", PNG_DEFAULT);
  // SLD_Surface *currImage = SLD_LoadBMP("texture_test.bmp");
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, currImage)	
//  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
}


void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* See drawplant.c for the definition of this routine */
	drawPlant(depth,thetaOffset);


    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
  switch(ch) {
  case 'a':
    printf("increasing depth.\n");
    depth++;
    display();
    break;
  case 's':
    printf("decreasing depth.\n");
    if(depth > 0)
      depth--;
    display();
    break;
  case '/':
    thetaOffset += -5;
    display();
    break;
  case '?':
    thetaOffset += 5;
    display();
    break;
  default:
    printf("heeeeyyy\n");
    break;
  }
}


/* end of plant.c */
