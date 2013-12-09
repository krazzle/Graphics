/******************************************************************/
/*         Geometry functions                                     */
/*                                                                */
/* Group Members: <FILL IN>                                       */
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
#include "raytrace.h"

extern item** sceneItems;
extern int numItems;
extern GLfloat dotProduct(vector*, vector*);

point* makePoint(GLfloat x, GLfloat y, GLfloat z) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));
  /* put stuff in it */
  p->x = x; p->y = y; p->z = z; 
  p->w = 1.0;
  return (p);
}

/* makes copy of point (or vector) */
point* copyPoint(point *p0) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));

  p->x = p0->x;
  p->y = p0->y;
  p->z = p0->z;
  p->w = p0->w;         /* copies over vector or point status */
  return (p);
}

/* unallocates a point */
void freePoint(point *p) {
  if (p != NULL) {
    free(p);
  }
}

/* vector from point p to point q is returned in v */
void calculateDirection(point* p, point* q, point* v) {
  v->x = q->x - p->x;
  v->y = q->y - p->y;
  v->z = q->z - p->z;
  /* a direction is a point at infinity */
  v->w = 0.0;

  /* NOTE: v is not unit length currently, but probably should be */
}

/* given a vector, sets its contents to unit length */


/* point on ray r parameterized by t is returned in p */
void findPointOnRay(ray* r,double t,point* p) {
  p->x = r->start->x + t * r->dir->x;
  p->y = r->start->y + t * r->dir->y;
  p->z = r->start->z + t * r->dir->z;
  p->w = 1.0;
}


/* SPHERES */

sphere* makeSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
  //printf("making sphere... its item number %d\n", numItems);
  sphere* s;
  /* allocate memory */
  s = (sphere*) malloc(sizeof(sphere));

  /* put stuff in it */
  s->c = makePoint(x,y,z);   /* center */
  s->r = r;   /* radius */
  s->m = NULL;   /* material */

  return(s);
}

plane* makePlane(GLfloat A, GLfloat B, GLfloat C, GLfloat x0, GLfloat y0, GLfloat z0){
  plane* p;
  p = (plane*) malloc(sizeof(plane));
  p->normal = (vector*) malloc(sizeof(vector));
  p->origin = (point*) malloc(sizeof(point));

  p->normal->x = A;
  p->normal->y = B;
  p->normal->z = C;
  p->origin->x = x0;
  p->origin->y = y0;
  p->origin->z = z0;
  p->m = NULL;

  return(p);
}

/* returns TRUE if ray r hits sphere s, with parameter value in t */
int raySphereIntersect(ray* r,sphere* s,double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - s->c->x;
  p.y = r->start->y - s->c->y;
  p.z = r->start->z - s->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  +  v->y * v->y  +  v->z * v->z;
  b = 2*( v->x * p.x  +  v->y * p.y  +  v->z * p.z);
  c = p.x * p.x + p.y * p.y + p.z * p.z - s->r * s->r;

  D = b * b - 4 * a * c;
  
  if (D < 0) {  /* no intersection */
    return (FALSE);
  }
  else {
    D = sqrt(D);
    /* First check the root with the lower value of t: */
    /* this one, since D is positive */
    *t = (-b - D) / (2*a);
    /* ignore roots which are less than zero (behind viewpoint) */
    if (*t < 0) {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0) { return(FALSE); }
    else return(TRUE);
  }
}

int planeIntersect(ray* r, plane* p, double* t){

	GLfloat d = p->origin->x*p->normal->x + p->origin->y*p->normal->y + p->origin->z*p->normal->z;

//	printf("d: %f r->start (%f,%f,%f) r->dir(%f,%f,%f) p->normal (%f,%f,%f)\n",d, r->start->x, r->start->y, r->start->z, r->dir->x, r->dir->y,r->dir->z, p->normal->x, p->normal->y, p->normal->z);


	GLfloat top = d + (p->normal->x*r->start->x) + (p->normal->y*r->start->y) + (p->normal->z*r->start->z);
	GLfloat bottom = (p->normal->x*r->dir->x) + (p->normal->y*r->dir->y) + (p->normal->z*r->dir->z);
	top = -top;
	*t = top/bottom;
	//printf("*t: %f\n", *t);
	if( *t < 0) { return(FALSE); }
	else {return(TRUE);}

	printf("r->start (%f,%f,%f) r->dir(%f,%f,%f) p->normal (%f,%f,%f)\n", r->start->x, r->start->y, r->start->z, r->dir->x, r->dir->y,r->dir->z, p->normal->x, p->normal->y, p->normal->z);

	*t = -(dotProduct(r->start, p->normal) + d)/dotProduct(r->dir, p->normal);	
	if(*t < 0) { return(FALSE);}
	else return(TRUE);
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findSphereNormal(sphere* s, point* p, vector* n) {
  n->x = (p->x - s->c->x) / s->r;  
  n->y = (p->y - s->c->y) / s->r;
  n->z = (p->z - s->c->z) / s->r;
  n->w = 0.0;
}

void findPlaneNormal(plane* p, vector* n){

  
  //Glfloat val = sqrt( (p->a * p->a) + (p->b * p->b) + (p->c * p->c) );
  GLfloat val = sqrt((p->normal->x*p->normal->x) + (p->normal->y*p->normal->y) + (p->normal->z*p->normal->z)); 

  n->x = p->normal->x/val;
  n->y = p->normal->y/val;
  n->z = p->normal->z/val;
  n->w = 0.0;
}
