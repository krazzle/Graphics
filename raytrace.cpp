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
ray* firstHit(ray*,point*,vector*,material**, int);
void addItem(uint32_t, int,int);
extern vector* getReflection(vector*, vector*);
void sortByDepth(point*, int);/* local data */
double getDistance(item*, point*);

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
  s1->m = makeMaterial(1.0, 0, 1.0, .2, .6, .2, 4);
  s2->m = makeMaterial(1.0, 0.1, 0.0, .2, .6, .2, 4);
  s3->m = makeMaterial(0.9,.9,.9, .4,.3,.3,4);
  p1 = makePlane(0,0,0, 0,0,0);
  p1->m = makeMaterial(0.0,1.0,1.0, 0,1,0 ,2);
//  addItem((uint32_t)&p1, 1);
  addItem((uint32_t)&s1, 0, 1);
  addItem((uint32_t)&s2, 0, 2);
  addItem((uint32_t)&s3, 0, 3);
  sortByDepth(viewpoint, FAR);
  l1 = makeLight(0,2,2, 0,1,1, 1.0,0,0);
  //l2 = makeLight(-10,10,10,-.5,.5,.5,1.0,0,0);
  lights[0] = l1;
  //lights[1] = l2;
  num_lights = 1;
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

void sortByDepth(point* p, int type){
	int i;
	int sorted = 0;
	while(!sorted){
		sorted = 1;
		for( i = 0; i < numItems-1; i++){
			item* i1 = sceneItems[i];
			item* i2 = sceneItems[i+1];
			double i1Val = getDistance(i1, p);
			double i2Val = getDistance(i2, p);
			if(type == NEAR){
				if(i2Val < i1Val){
                                	sorted = 0;
                                	item* temp = sceneItems[i];
                                	sceneItems[i] = sceneItems[i+1];
                               	 	sceneItems[i+1] = temp;
                        	}
			}
			else if(type == FAR){
				if(i2Val > i1Val){
                                	sorted = 0;
                                	item* temp = sceneItems[i];
                                	sceneItems[i] = sceneItems[i+1];
                                	sceneItems[i+1] = temp;
                        	}	
			}
			else {};
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
                default: printf("type not found\n"); break;
     	}
	return val;
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
  ray* reflected_ray;

  p.w = 0.0;  /* inialize to, "no intersection" */
  reflected_ray = firstHit(r,&p,&n,&m, d);

  if (p.w != 0.0) {
    shade(&p,&n,m,r->dir,c,d,(light**)lights);  /* do the lighting calculations */
  } 
 
  if(d > 0 && p.w != 0){
	//sort by depth from current point?
	sortByDepth(&p, NEAR);
//	printf("recursing on ray (%f,%f,%f) -> (%f,%f,%f) \n", reflected_ray->start->x, reflected_ray->start->y, reflected_ray->start->z, reflected_ray->dir->x, reflected_ray->dir->y, reflected_ray->dir->z);
	traceRay(reflected_ray,c, d-1);
  }

}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
ray* firstHit(ray* r, point* p, vector* n, material* *m, int depth) {
  double t = 0;     /* parameter value at first hit */
  int* hit = (int*) malloc(sizeof(int)*numItems);   
  vector* reflection = NULL;
  ray* reflected_ray = (ray*)malloc(sizeof(ray));

  vector* ray_vec = (vector*)malloc(sizeof(vector));

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
				ray_vec->x = p->x - r->start->x;
				ray_vec->y = p->y - r->start->y;
				ray_vec->z = p->z - r->start->z;
				reflection = getReflection(n, ray_vec);
			}
			break;}
		case 1:{
			plane* pl = *((plane**)cur_item->ptr);
			hit[i] = planeIntersect(r, pl, &t);
			if(hit[i]){
				curItem = i;
				*m = pl->m;
				findPointOnRay(r, t, p);
				findPlaneNormal(pl, p, n);
			        ray_vec->x = p->x - r->start->x;
                                ray_vec->y = p->y - r->start->y;
                                ray_vec->z = p->z - r->start->z;

				reflection = getReflection(n, ray_vec);
			}	
			break;}
		default: printf("type not found\n"); break;
	}
  }
   
  int sum = 0;
  for(i = 0; i < numItems; i++)
	sum+= hit[i];

  if(sum == 0) { p->w = 0.0; }
  else{
	reflected_ray->start = p;
	reflected_ray->dir = reflection;
  }
  return reflected_ray;
}


