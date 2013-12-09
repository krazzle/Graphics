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
#include <sys/resource.h>


/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void traceRay(ray*,color*,int);
void drawScene(void);
ray** firstHit(ray*,point*,vector*,material**, int, color*);
void addItem(uint32_t, int,int);
extern vector* getReflection(vector*, vector*);
extern GLfloat dotProduct(vector* v1, vector* v2);
void sortByDepth(point*);/* local data */
double getDistance(item*, point*);
vector* SnellsLaw(vector*, vector*, material*);

item** sceneItems;
int numItems;
int curItem;

/* the scene: so far, just one sphere */
sphere* s1;
sphere* s2;
sphere* s3;
sphere* s4;
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

  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  rl.rlim_cur = kStackSize;
  setrlimit(RLIMIT_STACK, &rl);

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
  s2 = makeSphere(-.10, 0, -1.5, 0.15);
  s3 = makeSphere(-.15, 0, -2, .15);
  s4 = makeSphere(.15, .05, -1.0, .10);
  s1->m = makeMaterial(1.0, 0,   0,   .4, .5, 0, 4, .2, .2);
  s2->m = makeMaterial(0,   1.0, 0,   .4, .5, 0, 4, .2, .2);
  s3->m = makeMaterial(0,   0,   1.0, .4, .5, 0, 4, .2, .2);
  s4->m = makeMaterial(1.0, 1.0, 1.0, .4, .5, 0, 4, .2, .2);

  p1 = makePlane( 1, 1, 1, 0, 0, 0);
  p1->m = makeMaterial(1.0, 1.0, 1.0, .4, .5, 0, 4, .2, .2);
  // end of plane attempt

  

  addItem((uint32_t)&s1, 0, 1);
  addItem((uint32_t)&s2, 0, 2);
  addItem((uint32_t)&s3, 0, 3);
  addItem((uint32_t)&s4, 0, 4);
  addItem((uint32_t)&p1, 1, 5);
  // sortByDepth(viewpoint); ******
  l1 = makeLight(0,2,2, 0,1,1, 1.0,0,0);
  l2 = makeLight(-10,10,10,-.5,.5,.5,1.0,0,0);
  lights[0] = l1;
  lights[1] = l2;
  num_lights = 2;
  curItem = numItems+1;
}

void addItem(uint32_t ptr, int type, int id_val){
	item* i = (item*)malloc(sizeof(item));
	i->ptr= ptr;
 	i->type = type;
	i->ID = id_val;
	sceneItems[numItems] = i;
	numItems++;
}

void sortByDepth(point* p){
	int i;
	int sorted = 0;
	while(!sorted){
		sorted = 1;
		for( i = 0; i < numItems-1; i++){
			item* i1 = sceneItems[i];
			item* i2 = sceneItems[i+1];
			double i1Val = getDistance(i1, p);
			double i2Val = getDistance(i2, p);
			if(i2Val < i1Val){
                                sorted = 0;
                                item* temp = sceneItems[i];
                                sceneItems[i] = sceneItems[i+1];
                         	sceneItems[i+1] = temp;
                        }
		}
	}
}

double getDistance(item *i, point* p){
  double val = 0;
  switch(i->type){
  case 0: {
    sphere* s = *((sphere**)i->ptr);
    val = sqrt(((s->c->x - p->x)*(s->c->x - p->x)) + ((s->c->y - p->y)*(s->c->y - p->y)) + ((s->c->z - p->z)*(s->c->z - p->z)));
  } break;
  case 1: {
    
    plane* pl = *((plane**)i->ptr);
    // distance = magnitude ( v dotProduct n), where v: dist(p,plane->origin) and n: normalize(plane->noramal)
    // v is p
    // vector n = findPlaneNormal(pl, &n);
    // val = dotProduct( n, (vector*)p);
    if ( val < 0) {
      val = -1 * val;
    }
    
  } break;	  
  default: printf("type not found\n"); break;		  
  }
  return val;
}

//*** move this function somewhere else


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
      //printf("calculating pixel (%f,%f)\n", worldPix.x, worldPix.y);

      c.r = 0;
      c.g = 0;
      c.b = 0;
      curItem = numItems+1;
      /* trace the ray! */
      traceRay(&r,&c,1);
      /* write the pixel! */
      drawPixel(i,j,c.r,c.g,c.b);
    }
  }
  printf("done drawing\n");
}

/* returns the color seen by ray r in parameter c */
/* d is the recursive depth */
void traceRay(ray* r, color* c, int d)  {

  //printf("in trace ray...\n");
 // curItem = numItems+1;
  point p;  /* first intersection point */
  vector n;
  material* m;
  ray* reflected_ray = NULL;
  ray* refracted_ray = NULL;
  ray** rays;

  p.w = 0.0;  /* inialize to, "no intersection" */
//  printf("ray(%f,%f,%f)->(%f,%f,%f)\n", r->start->x, r->start->y, r->start->z, r->dir->x, r->dir->y, r->dir->z);
//  refracted_ray = firstHit(r,&p,&n,&m, d,c, RETURN_REFRACTED);
  rays = firstHit(r,&p,&n,&m, d,c);

  if (p.w != 0.0) {
    shade(&p,&n,m,r->dir,c,d,(light**)lights);  /* do the lighting calculations */
  } 
  if(d > 0 && p.w != 0){
//	traceRay(rays[1], c, d-1);
	if(rays[0] != NULL)
		traceRay(rays[0], c, d);
	traceRay(rays[1], c, d-1);
  }
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
ray** firstHit(ray* r, point* p, vector* n, material* *m, int depth, color* c) {
  double t = 0;     /* parameter value at first hit */
  int* hit = (int*) malloc(sizeof(int)*numItems);   
  vector* reflection = NULL;
  ray* reflected_ray;
  ray* refracted_ray;
  ray** rays = (ray**)malloc(sizeof(ray*));

  int i; 
  for(i = 0; i < numItems; i++){
	if(i == curItem) //we dont want to check for an intersection on an item we've recently hit
		continue;
	item* cur_item = sceneItems[i];
        switch(cur_item->type){
		case 0: {
   			sphere* s = *((sphere**)cur_item->ptr);
			hit[i] = raySphereIntersect(r, s, &t);
			if(hit[i]){
				curItem = i;
				*m = s->m;
				findPointOnRay(r,t,p);
				findSphereNormal(s,p,n);				
				if(s->m->transparency > 0){
					refracted_ray = (ray*)malloc(sizeof(ray));
					refracted_ray->start = p;
					refracted_ray->dir = r->dir; 
					rays[0] = refracted_ray;
				}
				
				reflected_ray = (ray*)malloc(sizeof(ray));	
				vector* ray_vec = (vector*)malloc(sizeof(vector));
				ray_vec->x = p->x - r->start->x;
				ray_vec->y = p->y - r->start->y;
				ray_vec->z = p->z - r->start->z;
				reflection = getReflection(n, ray_vec);
				reflected_ray->start = p;
				reflected_ray->dir = reflection;	
				rays[1] = reflected_ray;
				return rays;
			}
			break;}
		case 1:{
		        plane* pl = *((plane**)cur_item->ptr);
			hit[i] = planeIntersect(r, pl, &t);
			if(hit[i]){
				curItem = i;
				*m = pl->m;
				findPointOnRay(r, t, p);
				findPlaneNormal(pl, n);
				if(pl->m->transparency > 0){
				  
				  refracted_ray = (ray*)malloc(sizeof(ray));
				  refracted_ray->start = p;
				  refracted_ray->dir = r->dir; 
				  rays[0] = refracted_ray;
				}
				
				reflected_ray = (ray*)malloc(sizeof(ray));	
				vector* ray_vec = (vector*)malloc(sizeof(vector));
				ray_vec->x = p->x - r->start->x;
				ray_vec->y = p->y - r->start->y;
				ray_vec->z = p->z - r->start->z;
				reflection = getReflection(n, ray_vec);
				reflected_ray->start = p;
				reflected_ray->dir = reflection;	
				rays[1] = reflected_ray;


				return rays;
			}	
			break;}
		default: printf("type not found\n"); break;
	}
  }
   
  p->w = 0.0;
  return NULL;
}

vector* SnellsLaw(vector* a, vector* b, material* m){
//	printf("a (%f,%f,%f) b(%f,%f,%f)\n", a->x, a->y, a->z, b->x, b->y, b->z);
//	GLfloat a_size = sqrt(((a->x)*(a->x)) + ((a->y)*(a->y)) + ((a->z)*(a->z)));
//	GLfloat b_size = sqrt(((b->x)*(b->x)) + ((b->y)*(b->y)) + ((b->z)*(b->z)));
/*	GLfloat k1 = 1;
	GLfloat k2 = 1.5;
		
	a->x = -(a->x);
	a->y = -(a->y);
	a->z = -(a->z);

	GLfloat angle1 = acos(dotProduct(a, b));
//	printf("sin(angle1) = %f (sin(angle1)/k2) = %f\n", sin(angle1), sin(angle1)/k2);
	GLfloat angle2 = asin((k1*sin(angle1))/k2);

	b->x = -(b->x);
	b->y = -(b->y);
	b->z = -(b->z);
	
//	printf("angle1 = %f, angle2 = %f\n", angle1, angle2);
*/
	vector* retval = (vector*) malloc(sizeof(vector));

//	retval->x = 	(cos(angle2)*cos(angle2))*(b->x) + 
//			(cos(angle2)*sin(angle2))*(b->y) + 
//			(-sin(angle2))*(b->z);

//	retval->y = 	((sin(angle2)*sin(angle2)*cos(angle2) - (cos(angle2)*sin(angle2)))*(b->x)) + 
//			((sin(angle2)*sin(angle2)*sin(angle2) + (cos(angle2)*cos(angle2)))*(b->y)) + 
//			(sin(angle2)*cos(angle2))*(b->z);

//	retval->z = 	((cos(angle2)*sin(angle2)*cos(angle2) + (sin(angle2)*sin(angle2)))*(b->x)) + 
//			((cos(angle2)*sin(angle2)*sin(angle2) - (sin(angle2)*cos(angle2)))*(b->y)) + 
//			(cos(angle2)*cos(angle2))*(b->z);

	retval->x = a->x;
	retval->y = a->y;
	retval->z = a->z;
//	printf("final angle (%f,%f,%f)\n", retval->x, retval->y, retval->z);
	return retval;
}
