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

extern int num_lights;
extern item** sceneItems;
extern int numItems;
extern int curItem;
extern int firstHit(ray*,point*,vector*,material**);
void normalize(vector* v);
GLfloat specularReflection(color* SpecularIntensity,vector* normal,  vector* light, vector* viewVector, material* m);
GLfloat dotProduct(vector* v1, vector* v2);
void ambientReflection(color *c , material* m);
GLfloat diffuseReflection(color* DiffuseIntensity,vector* normalVector, light* lightVector, material* m);
vector* getReflection(vector* normal, vector* light);
GLfloat comp_pow(GLfloat a, GLfloat b);
int shadow(point* p, vector* l); 

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat dif, GLfloat spec, GLfloat s, GLfloat refl, GLfloat trans) {

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
  m->reflectivity = refl;
  m->transparency = trans;
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
void shade(point* p, vector* n, material* m, vector* in, color* c, int d, light** lights) {

  GLfloat Kt = m->transparency;
  GLfloat Kd = 1;
  if(d == 0)
	Kd = m->reflectivity;

 // printf("calling normalize of the ones\n");
  normalize(n);
  normalize(in);
 // printf("num_lights: %d\n", num_lights);

  color *Ia = (color*)malloc(sizeof(color));
  color *Id = (color*)malloc(sizeof(color));
  color *Is = (color*)malloc(sizeof(color));  

/*  c->r = 0;
  c->g = 0;
  c->b = 0;
*/

 // printf("in shade\n");
  //vector* reflectionVec = getReflection(n, l->r->dir);  
  int i; 
  for(i = 0; i < num_lights; i++){
	light* l = lights[i];
//	GLfloat dist = sqrt(((p->x - l->r->start->x)*(p->x - l->r->start->x)) + ((p->y - l->r->start->y)*(p->y - l->r->start->y)) + ((p->z - l->r->start->z)*(p->z - l->r->start->z)));
      	normalize(l->r->dir);
  	ambientReflection(Ia, m);
  	diffuseReflection(Id, n, l, m);
  	specularReflection(Is, n, l->r->dir ,in, m);

	//may need to reverse direction of light
//	vector* vec = (vector*)malloc(sizeof(vector));
//	vec->x = l->r->start->x - p->x;
//	vec->y = l->r->start->y - p->y;
//	vec->z = l->r->start->z - p->z;
//	GLfloat distanceProd = getDistance(n, vec);
//	vec->x =  
//	GLfloat distanceProdPhong = 1;

	c->r += (Kt+Kd)*(Ia->r);
	c->g += (Kt+Kd)*(Ia->g);
	c->b += (Kt+Kd)*(Ia->b);

//	printf("before shadow\n");
	if(!shadow(p, l->r->start))	{
  		c->r += (Kt+Kd)*((Id->r) + (Is->r)*2);
  		c->g += (Kt+Kd)*((Id->g) + (Is->g)*2);
  		c->b += (Kt+Kd)*((Id->b) + (Is->b)*2);
	}
	

  }

  c->r/=num_lights;
  c->g/=num_lights;
  c->b/=num_lights; 

  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

//  if(d == 0)
//	printf("color value is (%f,%f,%f)\n", c->r, c->g, c->b);


}

int shadow(point* p, vector* l){ 
	ray* r = (ray*)malloc(sizeof(ray));
	r->start = (point*)malloc(sizeof(point));
	r->start->x = p->x;
	r->start->y = p->y;
	r->start->z = p->z;
	r->start->w = 1.0;
	r->dir = (vector*)malloc(sizeof(vector));
	r->dir->x = l->x;
	r->dir->y = l->y;
	r->dir->z = l->z;
	r->dir->w = 0.0;
	double t = 0;	
	
 	int* hit = (int*) malloc(sizeof(int)*numItems);
	int i;
	for(i = 0; i < numItems; i++){
		if(i == curItem)
			continue;
		item* cur_item = sceneItems[i];
		switch(cur_item->type){
			case 0: {
				sphere* s = *((sphere**)cur_item->ptr);
				hit[i] = raySphereIntersect(r, s, &t);
				break;
			}
			case 1: {
				plane* pl = *((plane**)cur_item->ptr);
				hit[i] = planeIntersect(r, pl, &t);
				break;
			}
  			case 2: {
				cylendar* c1 = *((cylendar**)cur_item->ptr);
  				hit[i] = rayCylendarIntersect(r, c1, &t);
				break;
			} 
			default: {printf("NEITHER\n"); break; }
		}
	}
	
	int sum = 0;
	for(i = 0; i < numItems; i++)
		sum+= hit[i];
	
//	printf("p (%f,%f,%f) sum: %f\n",p->x,p->y,p->z,sum);

	if(sum == 0) 
		return(FALSE);
	else if( sum != 0){
	//	printf("p (%f,%f,%f) sum: %d\n",p->x,p->y,p->z,sum);
		return(TRUE);
	}
	else
		return(FALSE);
}

void ambientReflection(color *c , material* m){
  c->r = m->amb*m->r;
  c->g = m->amb*m->g;
  c->b = m->amb*m->b;
}

GLfloat diffuseReflection(color* DiffuseIntensity, vector* normalVector, light* lightVector, material* m) {

  GLfloat max = 0;
  GLfloat dot_product = dotProduct(normalVector, lightVector->r->dir);
  if ( dot_product > 0 ) max = dot_product;
  
  DiffuseIntensity->r = m->dif * max * m->r;
  DiffuseIntensity->g = m->dif * max * m->g;
  DiffuseIntensity->b = m->dif * max * m->b;
  
  return max;
}

GLfloat specularReflection(color* SpecularIntensity,vector* normal, vector* light, vector* viewVector, material* m) {
 // printf("reflection (%f,%f,%f) view (%f,%f,%f)i\n", reflectionVector->x, reflectionVector->y, reflectionVector->z, viewVector->x, viewVector->y, viewVector->z);  
  //printf("getting reflection\n");
  vector* reflectionVec = getReflection(normal, light);  

 // printf("dot product\n");  
  GLfloat pizza = dotProduct(viewVector, reflectionVec);
  GLfloat max = 0;
  if( pizza > 0) 
  	max = pizza;

 // printf("computing power\n");
  GLfloat powVal = comp_pow(max, m->s);
  
  SpecularIntensity->r = m->spec * powVal * m->r;
  SpecularIntensity->g = m->spec * powVal * m->g;
  SpecularIntensity->b = m->spec * powVal * m->b;
  return max;
}

GLfloat comp_pow(GLfloat a, GLfloat b){
	GLfloat val = a;
	int i;
	for(i = b; i > 0; i--)
		val*=a;

//	printf("%f ^ %f = %f\n", a,b,val); 
	return val;
}


GLfloat dotProduct(vector* v1, vector* v2) {
  //printf(" (%f,%f,%f) . (%f,%f,%f) \n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z);
  GLfloat val = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);

//  printf("dot product: (%f,%f,%f) . (%f,%f,%f) = %f\n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, val);
  return val;
}

vector* getReflection(vector* normal, vector* light){
	GLfloat dot = dotProduct(normal, light);
        vector* reflection = (vector*)malloc(sizeof(vector));
    
        reflection->x = -((2*dot*normal->x) - light->x);
        reflection->y = -((2*dot*normal->y) - light->y);
	reflection->z = -((2*dot*normal->z) - light->z);
  	reflection->w = 0;

        normalize(reflection);
	return reflection;
}

void normalize(vector* v) {
  GLfloat length = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  v->x = v->x/length;
  v->y = v->y/length;
  v->z = v->z/length;
}


