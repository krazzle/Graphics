/******************************************************************/
/*         Main raytracer file                                    */
/*                                                                */
/* Group Members: Alyssa Sallean Andoni Mendoza                   */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "lowlevel.h"
#include "raytrace.h"

/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void traceRay(ray*,color*,int);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**);
void addItem(uint32_t, int);

/* local data */
item** sceneItems;
int numItems;
int curItem;

/* the scene: so far, just one sphere */
sphere* s1;
sphere* s2;
sphere* s3;
light* l1;
light* l2;
plane* p1;


light** lights;
int num_lights;
/* the viewing parameters: */
point* viewpoint;
GLfloat pnear;  /* distance from viewpoint to image plane */
GLfloat fovx;  /* x-angle of view frustum */
int width = 500;     /* width of window in pixels */
int height = 350;    /* height of window in pixels */

int main (int argc, char** argv) {
  int win;

  glutInit(&argc,argv);
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  win = glutCreateWindow("raytrace");
  glutSetWindow(win);
  init(width,height);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void init(int w, int h) {
  printf("INIT\n");
  
  /* OpenGL setup */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1.0, 0, 1.0, -1.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);  

  /* low-level graphics setup */
  initCanvas(w,h);

  /* raytracer setup */
  initCamera(w,h);
  initScene();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawScene();  /* draws the picture in the canvas */
  flushCanvas();  /* draw the canvas to the OpenGL window */
  glFlush();
}

void initScene () {
  
  lights = (light**) malloc(sizeof(light*));
  sceneItems = (item**) malloc(sizeof(item*)*10);  

  numItems = 0;
  s1 = makeSphere(.20, -.05,-2.0,0.15);
  s2 = makeSphere(-.10, .15, -1.5, 0.15);
  s3 = makeSphere(-.15, 0, -2, .15);
  s1->m = makeMaterial(1.0, 0.1, 1.0, .4, .3, .3, 4);
  s2->m = makeMaterial(1.0, 0.1, 0.0, .4, .3, .3, 4);
  s3->m = makeMaterial(0.0,1.0,.3, .4,.3,.3,4);
  p1 = makePlane(0,0,0, 0,0,0);
  p1->m = makeMaterial(0.0,1.0,1.0, 0,1,0 ,2);
//  addItem((uint32_t)&p1, 1);
  addItem((uint32_t)&s1, 0);
  addItem((uint32_t)&s3, 0);
  addItem((uint32_t)&s2, 0);
  
  l1 = makeLight(0,2,2, 0,1,1, 1.0,0,0);
  l2 = makeLight(-10,10,10,-.5,.5,.5,1.0,0,0);
  lights[0] = l1;
  lights[1] = l2;
  num_lights = 2;
//`  lights[1] = l2;
}

void addItem(uint32_t ptr, int type){
	item* i = (item*)malloc(sizeof(item));
	i->ptr= ptr;
 	i->type = type;
	sceneItems[numItems] = i;
	numItems++;
}

void initCamera (int w, int h) {
  viewpoint = makePoint(0.0,0,0);
  pnear = 1.0;
  fovx = PI/6;
}

void drawScene () {
  int i,j;
  GLfloat imageWidth;
  /* declare data structures on stack to avoid dynamic allocation */
  point worldPix;  /* current pixel in world coordinates */
  point direction; 
  ray r;
  color c;

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;

  imageWidth = 2*pnear*tan(fovx/2);

  /* trace a ray for every pixel */
  for (i=0; i<width; i++) {
    /* Refresh the display */
    /* Comment this line out after debugging */
    //flushCanvas();

    for (j=0; j<height; j++) {
      /* find position of pixel in world coordinates */
      /* y position = (pixel height/middle) scaled to world coords */ 
      worldPix.y = (j-(height/2))*imageWidth/width;
      /* x position = (pixel width/middle) scaled to world coords */ 
      worldPix.x = (i-(width/2))*imageWidth/width;

      /* find direction */
      /* note: direction vector is NOT NORMALIZED */
      calculateDirection(viewpoint,&worldPix,&direction);

      /* trace the ray! */
      traceRay(&r,&c,0);
      /* write the pixel! */
      drawPixel(i,j,c.r,c.g,c.b);
    }
  }
  printf("done drawing\n");
}

/* returns the color seen by ray r in parameter c */
/* d is the recursive depth */
void traceRay(ray* r, color* c, int d)  {
  point p;  /* first intersection point */
  vector n;
  material* m;

  p.w = 0.0;  /* inialize to, "no intersection" */
  firstHit(r,&p,&n,&m);

  if (p.w != 0.0) {
    shade(&p,&n,m,r->dir,c,d,(light**)lights);  /* do the lighting calculations */
  } else {             /* nothing was hit */
    c->r = 0.0;
    c->g = 0.0;
    c->b = 0.0;
  }
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m) {
  double t = 0;     /* parameter value at first hit */
  int* hit = (int*) malloc(sizeof(int)*numItems);   
  curItem = 2;  

  int i; 
  for(i = 0; i < numItems; i++){
	item* cur_item = sceneItems[i];
        switch(cur_item->type){
		case 0: {
 			//printf("generating sphere\n");
			//possibly wrong
			//cur_item->ptr is the address of the sphere pointer
   			sphere* s = *((sphere**)cur_item->ptr);
			hit[i] = raySphereIntersect(r, s, &t);
			if(hit[i]){
				curItem = i;
				*m = s->m;
				findPointOnRay(r,t,p);
				findSphereNormal(s,p,n);
			}
			break;}
		case 1:{
 	//		printf("drawing plane\n");
			//possibly wrong casting...
			plane* pl = *((plane**)cur_item->ptr);
			hit[i] = planeIntersect(r, pl, &t);
			if(hit[i]){
				curItem = i;
				*m = pl->m;
				findPointOnRay(r, t, p);
				findPlaneNormal(pl, p, n);
			}	
			break;}
		default: printf("type not found\n"); break;
	}
  }
   
  int sum = 0;
  for(i = 0; i < numItems; i++)
	sum+= hit[i];

  if(sum == 0) { p->w = 0.0;}

}

