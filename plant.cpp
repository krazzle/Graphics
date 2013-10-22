 
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

#include "helper.h"
#include "l-system-operators.h"
#include "common.h"
#include "drawplant.h"
#include <unistd.h>


/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
#define MOUSE_ROTATE_YX         0
#define MOUSE_ROTATE_YZ         1
#define MOUSE_ZOOM              2
int Y_OFF = 10;	/* window y offset */
int depth = 5;
GLfloat  thetaOffset = -40;
int perspective = 0;      // if 0, using perspective projection matrix,
                          // if 1, using orthographic projection matrix
int rising = 0;           // if 0, decrease depth during animation,
                          // if 1, increase depth during anitmation
int growingAnimation = 0; // if 0, do not use growing animation
                          // if 1, use growing animation
int season = 0;
GLUnurbsObj *theNurb;

/* local function declarations */
void display(void);
void init(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void myMouseMotion(int x, int y);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMotionFunc(myMouseMotion);
  glEnable(GL_MAP1_VERTEX_3);
  glutMainLoop();
  return 0;
}

int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;


void myMouseButton(int button, int state, int x, int y) {
        if (state == GLUT_DOWN) {
                m_last_x = x;
                m_last_y = y;

                if (button == GLUT_LEFT_BUTTON) {
                        mouse_mode = MOUSE_ROTATE_YX;
                } else if (button == GLUT_MIDDLE_BUTTON) {
                        mouse_mode = MOUSE_ROTATE_YZ;
                } else if (button == GLUT_RIGHT_BUTTON) {
                        mouse_mode = MOUSE_ZOOM;
                }
        }
}

void myMouseMotion(int x, int y) {
        double d_x;
        d_x = x - m_last_x;
        m_last_x = x;
        if (mouse_mode == MOUSE_ROTATE_YX) {
		//printf("d_x: %f\n", d_x);
                if(d_x > 0)
			thetaOffset -= 2;
		else 
			thetaOffset +=2;
        }
        display();
}

void init() {

   glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);


	   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
}


void display() {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* See drawplant.c for the definition of this routine */
  drawPlant(depth,thetaOffset, season);
  glFlush();  /* Flush all executed OpenGL ops finish */


  if (growingAnimation) {
    if(rising == 0) {
      if (depth == 0) {
	rising = 1;
	depth += 1;
      }
      else
	depth += -1;
    }
    else {
      if (depth == 6) {
	rising = 0;
	depth += -1;
      }
      else 
	depth += 1;
    }
    usleep(400000);
    glutPostRedisplay();
  }
  
  /*
   * Since we are using double buffers, we need to call the swap
   * function every time we are done drawing.
   */
  glutSwapBuffers();

}


//void display() {
//  glEnable(GL_DEPTH_TEST);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  
  /* See drawplant.c for the definition of this routine */
//  drawPlant(depth,thetaOffset);
  
  
//  glFlush();  /* Flush all executed OpenGL ops finish */
  
  /*
   * Since we are using double buffers, we need to call the swap
   * function every time we are done drawing.
   */
//  glutSwapBuffers();
//}


void myKeyHandler(unsigned char ch, int x, int y) {
  switch(ch) {
  case 'a':
    printf("increasing depth.\n");
    depth++;
    display();
    break;
  case 's':
    //printf("decreasing depth.\n");
    if(depth > 0){
      depth--;
      printf("decreasing depth.\n");
    }
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
  case 'e':
    season++;
    if(season == 4)
	season = 0;
    switch(season){
        case 0: printf("It's fall!\n");break;
	case 1: printf("It's winter!\n");break;
	case 2: printf("It's summer!\n");break;
	case 3: printf("It's spring!\n"); break;
	default: break;
    }
    display();
    break;
  case 'p':
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(perspective == 0) {
      gluPerspective(120.0f, 1.0f, 1.0f, 30.0f);
      gluLookAt(10, 15, 10,
		0, 10, 0,
		0, 1, 0
		);
      perspective = 1;
      printf("using perspective projection matrix\n");
    }
    else {
      glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
      perspective = 0;
      printf("using orthographic projection matrix\n");
    }
    display();
    break;
  case 'g':
    if(growingAnimation == 0) {
      growingAnimation = 1;
      printf("growing animation\n");
    }
    else
      growingAnimation = 0;
    display();
    break;
  default:
    printf("heeeeyyy\n");
    break;
  }
}


/* end of plant.c */
