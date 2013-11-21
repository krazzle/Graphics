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

void specularReflection(vector* outgoingSpecularIntensity, GLfloat specularCoef, vector* reflectionVector, vector* viewVector, vector* incomingSpecularIntensity);
void diffuseReflection(vector* outgoingDiffuseIntensity, GLfloat diffuseCoef, vector* normalVector, vector* lightVector, vector* incomingDiffuseIntensity)

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
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

  l->r = r;
  l->g = g;
  l->b = b;

  return(l);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, int d, light* l) {

  /* so far, just finds ambient component of color */
  c->r = m->amb * m->r;
  c->g = m->amb * m->g;
  c->b = m->amb * m->b;
  
  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}


void diffuseReflection(vector* outgoingDiffuseIntensity, GLfloat diffuseCoef, vector* normalVector, vector* lightVector, vector* incomingDiffuseIntensity) {
  
  GLfloat max = 0;
  if ( 0 < dotProduct(normalVector, lightVector) )
    max = dotProduct(normalVector, lightVector);
  
  outgoingDiffuseIntensity->x = diffuseCoef *
    max * 
    incomingDiffuseIntensity->x;
  
  outgoingDiffuseIntensity->y = diffuseCoef *
    max * 
    incomingDiffuseIntensity->y;
  
  outgoingDiffuseIntensity->z = diffuseCoef *
    max * 
    incomingDiffuseIntensity->z;
  
}

void specularReflection(vector* outgoingSpecularIntensity, GLfloat specularCoef, vector* reflectionVector, vector* viewVector, vector* incomingSpecularIntensity) {
  
  GLfloat pizza = dotProduct( reflectionVector, viewVector);
  
  outgoingSpecularIntensity->x = specularCoef *
    pizza *
    incomingSpecularIntensity->x;

  outgoingSpecularIntensity->y = specularCoef *
    pizza *
    incomingSpecularIntensity->y;
  
  outgoingSpecularIntensity->z = specularCoef *
    pizza *
    incomingSpecularIntensity->z;
  
}


GLfloat dotProduct(vector* v1, vector* v2) {
  
  return ( v1->x * v2->x ) + 
    ( v1->y * v2->y ) + 
    ( v1->z * v2->z ); 
  
}





