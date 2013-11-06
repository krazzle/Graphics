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

#include "helper.cpp"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

int drawStyleState = 0;
int faceOrPoints = 0;
/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;

GLfloat cpts[960][3][3];
int ncpts = 0;
int totalPoints = ncpts;
int threeDmode = 0;
int numOfVerticalSubs = 1;
int numOfHorizontalSubs = 0;
int offset = 32;

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);
void displayPointsAndLines();
void displayRotatedPointsAndLines();
void phil(void);
void VerticalSubdivide();

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
	
//	int i, j;
//	for(i = 0; i < 960; i++)
//		for(j = 0; j < 3; j++) 
//			cpts[i][j][0] = -99;
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
  if (threeDmode)
    displayRotatedPointsAndLines();
  else
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
    if ( ncpts >= 5) {
      threeDmode = 1;
      phil();
    } else
      printf("include at least five points\n");
    break;
  case 'e':
    if ( drawStyleState == 0)
      drawStyleState = 1;
    else  
      drawStyleState = 0;
    display();
    break;
  case 'r':
    if ( faceOrPoints == 0)
      faceOrPoints = 1;
    else
      faceOrPoints = 0;
    display();
    break;
  case 'a':
    VerticalSubdivide();
    display();
    break;
  default:
    /* Unrecognized keypress */
    return;
  }
  
  glutPostRedisplay(); 
  return;
}

void VerticalSubdivide(){
	if(numOfVerticalSubs == 5)
		return;
	numOfVerticalSubs++;
	int old_offset = offset;
	offset/=2;
	int i;
	for(i = 0;i < ncpts*32; i+=old_offset){
		if((i +old_offset) >= ncpts*32)
			continue;
	
		
 
		cpts[i+offset][0][0] = (cpts[i][0][0] + cpts[i+old_offset][0][0])/2;
		cpts[i+offset][0][1] = (cpts[i][0][1] + cpts[i+old_offset][0][1])/2;
		cpts[i+offset][0][2] = (cpts[i][0][2] + cpts[i+old_offset][0][2])/2;
	}
	phil();
	totalPoints = (2*totalPoints -1);
	//printf("total points changed to %d\n", totalPoints);
}


void phil(void) {
  
 // printf("in phil\n");

  GLfloat degreesToRads = (1*3.141592654)/180;
  GLfloat trad = degreesToRads * 120;
  
  // rotation matrix for 120 degrees
  GLfloat m11[4] = { cos(trad), 0, sin(trad), 0};
  GLfloat m12[4] = { 0, 1, 0, 0};
  GLfloat m13[4] = { -1*sin(trad), 0, cos(trad), 0};
  GLfloat m14[4] = { 0, 0, 0, 1};
  GLfloat *m1[4] = { m11, m12, m13, m14};
  
  // rotation matrix for -120 degrees
  GLfloat m21[4] = { cos(-trad), 0, sin(-trad), 0};
  GLfloat m22[4] = { 0, 1, 0, 0};
  GLfloat m23[4] = { -sin(-trad), 0, cos(-trad), 0};
  GLfloat m24[4] = { 0, 0, 0, 1};
  GLfloat *m2[4] = { m21, m22, m23, m24};
  
  // vector for an arbritray point in cpts
  GLfloat v1[1] = {0};
  GLfloat v2[1] = {0};
  GLfloat v3[1] = {0};
  GLfloat v4[1] = {1};
  GLfloat *v[4] = { v1, v2, v3, v4};

  int i, j;
  for ( i = 0; i < ncpts*32; i += offset) { 
   
    //printf("ncpts: %d\n", i);

    // get point
    v[0][0] = cpts[i][0][0];
    v[1][0] = cpts[i][0][1];
    v[2][0] = cpts[i][0][2];
    v[3][0] = 1;

    // rotate by m1, which is 120 degrees
   // printf("first multiplay start\n");
    GLfloat** result = multiply(4, 4, m1, 4, 1, v);
   // printf("first multiply end\n");
    // put it back in there after rotated
    cpts[i][1][0] = result[0][0];
    cpts[i][1][1] = result[1][0];
    cpts[i][1][2] = result[2][0];

    // get point
    v[0][0] = cpts[i][0][0];
    v[1][0] = cpts[i][0][1];
    v[2][0] = cpts[i][0][2];
    v[3][0] = 1;
    
    
    // rotate by m2, which is -120 degrees
    //printf("second multiply start\n");
    result = multiply(4, 4, m2, 4, 1, v);
    //printf("second multiply end\n");
    // put it back in there after rotated
    cpts[i][2][0] = result[0][0];
    cpts[i][2][1] = result[1][0];
    cpts[i][2][2] = result[2][0];
  }
  
  
  //printf("about to display\n");
  display();
  //printf("out of display\n");
  //printf("out of phil\n");
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
        for (i = 0; i < ncpts*32; i+=offset) {
	//  printf("%f %f\n", cpts[i][0], cpts[i][1]);
	  glVertex3fv(cpts[i][0]);
	}
        glEnd();

	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
    	for (i = 0; i < ncpts*32; i+=offset)
        	glVertex3fv(cpts[i][0]);
    	glEnd();
}


void displayRotatedPointsAndLines(){
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3f(0, -40, 0);
	glVertex3f(0, 40, 0);
	glEnd();

	glColor3f(1.0,0.0,0.0);
	glPointSize(5);
	int i,j;	

	if ( drawStyleState == 0 && faceOrPoints == 0) {
	  for( i = 0; i < ncpts*32; i+=offset){
	    if((cpts[i][0][0] == -99) || cpts[i+offset][0][0] == -99)
	      continue;
	    glBegin(GL_POLYGON);
	    glVertex3fv(cpts[i][0]);
	    glVertex3fv(cpts[i][1]);
	    glVertex3fv(cpts[i+offset][1]);
	    glVertex3fv(cpts[i+offset][0]);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex3fv(cpts[i][0]);
	    glVertex3fv(cpts[i][2]);
	    glVertex3fv(cpts[i+offset][2]);
	    glVertex3fv(cpts[i+offset][0]);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex3fv(cpts[i][1]);
	    glVertex3fv(cpts[i][2]);
	    glVertex3fv(cpts[i+offset][2]);
	    glVertex3fv(cpts[i+offset][1]);
	    glEnd();
	  }
	}

	if ( drawStyleState == 1 && faceOrPoints == 0) {
	  for(i = 0; i < 3; i++){
	    glBegin(GL_LINE_STRIP);
	    for(j = 0; j < totalPoints*offset;j+=offset){
	        glVertex3fv(cpts[j][i]);
	    }
	    glEnd();
	  }

          for (i = 0; i < totalPoints*offset; i+=offset) {
	    glBegin(GL_LINE_STRIP);
	    for (j = 0; j < 3; j++){
	        glVertex3fv(cpts[i][j]);
	    }
	    glEnd();
	  }
	}

	if ( drawStyleState == 0 && faceOrPoints == 1 
	     || (drawStyleState == 1 && faceOrPoints == 1)) {
	  glColor3f(0.0,0.0,1.0);
	  //glBegin(GL_POINTS);
	  for (i = 0; i < ncpts*32; i+=offset) {
	    glBegin(GL_POINTS);
	    for (j = 0; j < 3; j++) {	
	      glVertex3fv(cpts[i][j]);
	    }
	    glEnd();
	  }
	}
	//glEnd();
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
	  totalPoints = ncpts;
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
    		cpts[ncpts*offset][0][0] = wx;
    		cpts[ncpts*offset][0][1] = wy;
    		cpts[ncpts*offset][0][2] = 0.0;
    		ncpts++;
		totalPoints = ncpts;
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
