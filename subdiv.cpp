/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
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

#include "common.h"
#include "drawing.h"
#include "data.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;

GLfloat cpts[30][3][3];
int ncpts = 0;


/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);
void displayPointsAndLines();

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
}

void display() {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  
  /*
   * See drawing.c for the definition of these routines.
   *
   * Note: Only one should be called at a time (based on the
   * display mode).
   */
  displayPointsAndLines();
  drawSurface();
  
  glFlush();  /* Flush all executed OpenGL ops finish */
  
  /*
   * Since we are using double buffers, we need to call the swap
   * function every time we are done drawing.
   */
  glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'q':
			endSubdiv(0);
			break;
	case 'w': 
	  phil();
	  break;
		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();

	return;
}

void phil(void) {
  
  GLfloat degreesToRads = (1*3.141592654)/180;
  GLfloat rX = x*degreesToRads;
  GLfloat rY = y*degreesToRads;
  GLfloat rZ = z*degreesToRads;

  Glfloat trad = degreeToRads * 120;
  
  GLfloat** m1 = new GLfloat*[4];
  int a;
  for ( a = 0; a < 4; a += 1)
    m1[a] = new GLfloat[4];
  m1[0] = { cos(trad), 0, sin(trad), 0};
  m1[1] = { 0, 1, 0, 0};
  m1[2] = { -1*sin(trad), 0, cos(trad), 0};
  m1[3] = { 0, 0, 0, 1}

  GLfloat** m2 = new GLfloat*[4];
  for ( a = 0; a < 4; a += 1)
    m2[a] = new GLfloat[4];
  m2[0] = { cos(-trad), 0, sin(-trad), 0};
  m2[1] = { 0, 1, 0, 0};
  m2[2] = { -sin(-trad), 0, cos(-trad), 0};
  m2[3] = { 0, 0, 0, 1}

  GLfloat** v = newGLfloat*[4];
  for ( a = 0; a < 4; a += 1) 
    v[a] = new GLfloat[1];
  v[3] = 1;
  int i, j;
  for ( i = 0; i < nctps; i += 1) { 

      v[0] = cpts[i][1][0];
      v[1] = cpts[i][1][1];
      v[2] = cpts[i][1][2];
      GLfloat** result = multiply(4, 4, m1, 4, 1, v);
      
      


  }


		   		   
}

void displayPointsAndLines(){
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3f(0, -40, 0);
	glVertex3f(0, 40, 0);
	glEnd();

	glColor3f(1.0,0.0,0.0);
	glPointSize(5);
	int i;
	glBegin(GL_LINE_STRIP);
        for (i = 0; i < ncpts; i++) {
	//  printf("%f %f\n", cpts[i][0], cpts[i][1]);
	  glVertex3fv(cpts[i][0]);
	}
        glEnd();

	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
    	for (i = 0; i < ncpts; i++)
        	glVertex3fv(cpts[i][0]);
    	glEnd();
}

void myMouseButton(int button, int state, int x, int y) { 
	float wx, wy;

    /* We are only interested in left clicks */

	//    	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
	//		return;
	if (state != GLUT_DOWN)
	  return;
	
	if (button == GLUT_RIGHT_BUTTON) {
	  ncpts += -1; 
	}
	else {

		if((x < ((W/2) + 5)) && (x > ((W/2) - 5)))
			x = W/2;

    		wx = (2.0 * x) / (float)(W - 1) - 1.0;
    		wy = (2.0 * (H - 1 - y)) / (float)(H - 1) - 1.0;
	
		wx*= fright;
		wy*= ftop;
    		if (ncpts == 30)
        		return;
    		cpts[ncpts][0][0] = wx;
    		cpts[ncpts][0][1] = wy;
    		cpts[ncpts][0][2] = 0.0;
    		ncpts++;
	}

    /* Draw the point */

    	glFlush();

	display();
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}


/* end of subdiv.cpp */
