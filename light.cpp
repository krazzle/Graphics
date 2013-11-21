/******************************************************************/
/*         Lighting functions                                     */
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

void specularReflection(color* SpecularIntensity, vector* reflectionVector, vector* viewVector, material* m);
GLfloat dotProduct(vector* v1, vector* v2);
void ambientReflection(color *c , material* m);
void diffuseReflection(color* DiffuseIntensity,vector* normalVector, light* lightVector, material* m);
vector* getReflection(vector* normal, vector* light);

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat dif, GLfloat spec, GLfloat s) {

  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
  m->dif = dif;
  m->spec = spec;
  m->s = s;
  return(m);
}

light* makeLight(GLfloat x, GLfloat y ,GLfloat z, GLfloat vx, GLfloat vy, GLfloat vz, GLfloat r, GLfloat g, GLfloat b){
  light *l;
  l = (light*) malloc(sizeof(light));
  l->r = (ray*) malloc(sizeof(ray));
  l->r->start = (point*)malloc(sizeof(point));
  l->r->dir = (vector*)malloc(sizeof(vector));  

  //location
  l->r->start->x = x;
  l->r->start->y = y;
  l->r->start->z = z;
  l->r->start->w = 1.0;
   
  //direction
  l->r->dir->x = vx;
  l->r->dir->y = vy;
  l->r->dir->z = vz;
  l->r->dir->w = 0.0;

  l->red = r;
  l->green = g;
  l->blue = b;

  return(l);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, int d, light* l, vector* direction) {


  color *Ia = (color*)malloc(sizeof(color));
  color *Id = (color*)malloc(sizeof(color));
  color *Is = (color*)malloc(sizeof(color));  

  vector* reflectionVec = getReflection(n, l->r->dir);  

  ambientReflection(Ia, m);
  diffuseReflection(Id, n, l, m);
  specularReflection(Is, reflectionVec ,direction, m);
 // specularReflection(Is, 

  c->r = Id->r;
  c->g = Id->g;
  c->b = Id->b;

  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}

void ambientReflection(color *c , material* m){
  c->r = m->amb*m->r;
  c->g = m->amb*m->g;
  c->b = m->amb*m->b;
}

void diffuseReflection(color* DiffuseIntensity, vector* normalVector, light* lightVector, material* m) {

  GLfloat max = 0;
  GLfloat dot_product = dotProduct(normalVector, lightVector->r->dir);
  if ( dot_product > 0 ) max = dot_product;
  
  DiffuseIntensity->r = m->dif * max * m->r;
  DiffuseIntensity->g = m->dif * max * m->g;
  DiffuseIntensity->b = m->dif * max * m->b;
  
}

void specularReflection(color* SpecularIntensity,vector* reflectionVector, vector* viewVector, material* m) {
  
  GLfloat pizza = dotProduct(reflectionVector, viewVector);
  
  SpecularIntensity->r = m->spec * pow(pizza, m->s) * m->r;
  SpecularIntensity->g = m->spec * pow(pizza, m->s) * m->g;
  SpecularIntensity->b = m->spec * pow(pizza, m->s) * m->b;
}

GLfloat pow(GLfloat a, GLfloat b){
	GLfloat val = a;
	int i;
	for(i = b; i > 0; i--)
		val*=a;
	return val;
}


GLfloat dotProduct(vector* v1, vector* v2) {
  GLfloat val = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);

//  printf("dot product: (%f,%f,%f) . (%f,%f,%f) = %f\n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, val);
  return val;
}

vector* getReflection(vector* normal, vector* light){
	GLfloat dot = dotProduct(normal, light);
        vector* reflection = (vector*)malloc(sizeof(vector));
        reflection->x = 2*dot*normal->x - light->x;
        reflection->y = 2*dot*normal->y - light->y;
	reflection->z = 2*dot*normal->z - light->z;
  	reflection->w = 0;
	return reflection;
}
