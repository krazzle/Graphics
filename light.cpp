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
void shade(point* p, vector* n, material* m, vector* in, color* c, int d, light* l) {


  color *Ia = (color*)malloc(sizeof(color));
  color *Id = (color*)malloc(sizeof(color));
  color *Is = (color*)malloc(sizeof(color));  

  ambientReflection(Ia, m);
  diffuseReflection(Id, n, l, m);
 // specularReflection(Is, 

  c->r = Ia->r;
  c->g = Ia->g;
  c->b = Ia->b;

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
  if ( 0 < dotProduct(normalVector, (vector*)lightVector->r->dir) )
    max = dotProduct(normalVector, (vector*)lightVector->r->dir);
  
  DiffuseIntensity->r = m->dif * max * m->r;
  DiffuseIntensity->g = m->dif * max * m->g;
  DiffuseIntensity->b = m->dif * max * m->b;
  
}

void specularReflection(color* SpecularIntensity,light* reflectionVector, vector* viewVector, material* m) {
  
//  GLfloat pizza = dotProduct( reflectionVector, viewVector);
  
//  SpecularIntensity->r =  * pow(pizza, m->s) * m->r;
//  SpecularIntensity->g = specularCoef * pow(pizza, m->s) * m->g;
 // SpecularIntensity->z = specularCoef * pow(pizza, m->s) * m->b;
}

GLfloat pow(GLfloat a, GLfloat b){
	GLfloat val = a;
	int i;
	for(i = b; i > 0; i--)
		val*=a;
	return val;
}


GLfloat dotProduct(vector* v1, vector* v2) {
  
  return ( v1->x * v2->x ) + 
    ( v1->y * v2->y ) + 
    ( v1->z * v2->z ); 
  
}





