#ifndef _VRML_H_
#define _VRML_H_

/* All variables / contants declared in this file are global. */

/*
 * Constants for the different types of VRML objects.
 * The constants should begin at 0 and should be consecutive integers.
 * 
 * VR_MAX should be the total number of vrml objects.
 */
#define VR_CUBE				0
#define VR_DODECAHEDRON		1
#define VR_ICOSAHEDRON		2
#define VR_PYRAMID			3
#define VR_MAX				4

/* The current vermal object */
extern int vr_object;


/* Function Declarations */
void draw_vrml_cube(int spikes);
void draw_vrml_dodecahedron(int spikes);
void draw_vrml_icosahedron(int spikes);
void draw_vrml_pyramid(int spikes);
/* Print the current vrml object.  Used when the user selects a new object. */
void print_vrml_object(void);
GLfloat* cross(GLfloat** vectors);
GLfloat* centerpoint(GLfloat** vectors, int num);
GLfloat* normalize(GLfloat* vector);
void make_spike(GLfloat** vectors, int num);
GLfloat* combine_vertices(GLfloat* a, GLfloat* b, int length, int is_add);


#endif	/* _VRML_H_ */

