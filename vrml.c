/*
 * vrml.c
 * ------
 * Drawing subroutines for each vrml object.
 * Also contains the polygon data for each vrml object.
 *
 * Starter code for Project 1.
 *
 * Group Members: <FILL IN>
 */

#include "common.h"

#include <stdio.h>

#include "vrml.h"

/*
 * Note that in VRML, the face indices have a -1 to represent
 * the termination of a index sequence.
 */

/***********************************************************
 * Begin VRML Cube Data
 ***********************************************************/
GLfloat v_cube_vertices[] = {
	1.632990, 0.000000, 1.154700,
	0.000000, 1.632990, 1.154700,
	-1.632990, 0.000000, 1.154700,
	0.000000, -1.632990, 1.154700,
	0.000000, -1.632990, -1.154700,
	1.632990, 0.000000, -1.154700,
	0.000000, 1.632990, -1.154700,
	-1.632990, 0.000000, -1.154700,
};

GLint v_cube_indices[] = {
	0, 1, 2, 3, -1,
	4, 5, 0, 3, -1,
	5, 6, 1, 0, -1,
	6, 7, 2, 1, -1,
	3, 2, 7, 4, -1,
	7, 6, 5, 4, -1,
};


/***********************************************************
 * Begin VRML Dodecahedron Data
 ***********************************************************/
GLfloat v_dodeca_vertices[] = {
	1.214120, 0.000000, 1.589310,
	0.375185, 1.154700, 1.589310,
	-0.982247, 0.713644, 1.589310,
	-0.982247, -0.713644, 1.589310,
	0.375185, -1.154700, 1.589310,
	1.964490, 0.000000, 0.375185,
	1.589310, 1.154700, -0.375185,
	0.607062, 1.868350, 0.375185,
	-0.607062, 1.868350, -0.375185,
	-1.589310, 1.154700, 0.375185,
	-1.964490, 0.000000, -0.375185,
	-1.589310, -1.154700, 0.375185,
	-0.607062, -1.868350, -0.375185,
	0.607062, -1.868350, 0.375185,
	1.589310, -1.154700, -0.375185,
	0.982247, 0.713644, -1.589310,
	0.982247, -0.713644, -1.589310,
	-0.375185, 1.154700, -1.589310,
	-1.214120, 0.000000, -1.589310,
	-0.375185, -1.154700, -1.589310,
};

GLint v_dodeca_indices[] = {
	0, 1, 2, 3, 4, -1,
	0, 5, 6, 7, 1, -1,
	1, 7, 8, 9, 2, -1,
	2, 9, 10, 11, 3, -1,
	3, 11, 12, 13, 4, -1,
	4, 13, 14, 5, 0, -1,
	15, 6, 5, 14, 16, -1,
	17, 8, 7, 6, 15, -1,
	18, 10, 9, 8, 17, -1,
	19, 12, 11, 10, 18, -1,
	16, 14, 13, 12, 19, -1,
	16, 19, 18, 17, 15, -1,
};


/***********************************************************
 * Begin VRML Icosahedron Data
 ***********************************************************/
GLfloat v_icosa_vertices[] = {
	0.552786, 1.701300, 0.894427,
	0.000000, 0.000000, 2.000000,
	1.788850, 0.000000, 0.894427,
	-1.447210, 1.051460, 0.894427,
	-1.447210, -1.051460, 0.894427,
	0.552786, -1.701300, 0.894427,
	1.447210, 1.051460, -0.894427,
	-0.552786, 1.701300, -0.894427,
	-1.788850, 0.000000, -0.894427,
	-0.552786, -1.701300, -0.894427,
	1.447210, -1.051460, -0.894427,
	0.000000, 0.000000, -2.000000,
};

GLint v_icosa_indices[] = {
	0, 1, 2, -1,
	3, 1, 0, -1,
	4, 1, 3, -1,
	5, 1, 4, -1,
	2, 1, 5, -1,
	0, 2, 6, -1,
	7, 0, 6, -1,
	3, 0, 7, -1,
	8, 3, 7, -1,
	4, 3, 8, -1,
	9, 4, 8, -1,
	5, 4, 9, -1,
	10, 5, 9, -1,
	6, 2, 10, -1,
	2, 5, 10, -1,
	6, 11, 7, -1,
	7, 11, 8, -1,
	8, 11, 9, -1,
	9, 11, 10, -1,
	10, 11, 6, -1,
};


/***********************************************************
 * Begin VRML Pyramid Data
 ***********************************************************/
GLfloat v_pyramid_vertices[] = {
	0.997029, 0.000000, -0.997029,
	0.012175, 1.000000, -0.012175,
	-0.997029, 0.000000, -0.997029,
	-0.012175, 1.000000, -0.012175,
	-0.997029, 0.000000, 0.997029,
	-0.012175, 1.000000, 0.012175,
	0.997029, 0.000000, 0.997029,
	0.012175, 1.000000, 0.012175,
};

GLint v_pyramid_indices[] = {
	6, 0, 7, -1,
	7, 0, 1, -1,
	0, 2, 1, -1,
	1, 2, 3, -1,
	2, 4, 3, -1,
	3, 4, 5, -1,
	4, 6, 5, -1,
	5, 6, 7, -1,
	4, 0, 6, -1,
	4, 2, 0, -1,
	7, 1, 5, -1,
	1, 3, 5, -1,
};


/*
 * ADD YOUR FUNCTION DEFINITIONS HERE.
 * See vrml.h for a list of suggested functions.
 */

void draw_vrml_cube()
{
	int i;
	glColor3f(1.0f, 1.0f, 0.0f);

	for(i = 0; i < 30; i+=5)
	{
		glBegin(GL_LINE_LOOP);
		int index1 = v_cube_indices[i] * 3;
		int index2 = v_cube_indices[i+1] * 3;
		int index3 = v_cube_indices[i+2] * 3;
		int index4 = v_cube_indices[i+3] * 3;
		
		glVertex3f(v_cube_vertices[index1], v_cube_vertices[index1+1], v_cube_vertices[index1+2]);
		glVertex3f(v_cube_vertices[index2], v_cube_vertices[index2+1], v_cube_vertices[index2+2]);
		glVertex3f(v_cube_vertices[index3], v_cube_vertices[index3+1], v_cube_vertices[index3+2]);
		glVertex3f(v_cube_vertices[index4], v_cube_vertices[index4+1], v_cube_vertices[index4+2]);
		glEnd();
	}
}

void draw_vrml_dodecahedron()
{

	int i;
	glColor3f(1.0f, 1.0f, 0.0f);
	for(i = 0; i < 72; i+=6)
	{
		glBegin(GL_LINE_LOOP);
		int index1 = v_dodeca_indices[i] * 3;
		int index2 = v_dodeca_indices[i+1] * 3;
		int index3 = v_dodeca_indices[i+2] * 3;
		int index4 = v_dodeca_indices[i+3] * 3;
		int index5 = v_dodeca_indices[i+4] * 3;
	
		glVertex3f(v_dodeca_vertices[index1], v_dodeca_vertices[index1+1], v_dodeca_vertices[index1+2]);
		glVertex3f(v_dodeca_vertices[index2], v_dodeca_vertices[index2+1], v_dodeca_vertices[index2+2]);
		glVertex3f(v_dodeca_vertices[index3], v_dodeca_vertices[index3+1], v_dodeca_vertices[index3+2]);
		glVertex3f(v_dodeca_vertices[index4], v_dodeca_vertices[index4+1], v_dodeca_vertices[index4+2]);
		glVertex3f(v_dodeca_vertices[index5], v_dodeca_vertices[index5+1], v_dodeca_vertices[index5+2]);
		glEnd();
	}
}

void draw_vrml_icosahedron()
{
	int i;

	glColor3f(1.0f, 1.0f, 0.0f);
	for(i = 0; i < 80; i+=4)
	{	
		glBegin(GL_LINE_LOOP);
		int index1 = v_icosa_indices[i]*3;
		int index2 = v_icosa_indices[i+1]*3;
		int index3 = v_icosa_indices[i+2]*3;

		glVertex3f(v_icosa_vertices[index1], v_icosa_vertices[index1+1], v_icosa_vertices[index1+2]);
		glVertex3f(v_icosa_vertices[index2], v_icosa_vertices[index2+1], v_icosa_vertices[index2+2]);
		glVertex3f(v_icosa_vertices[index3], v_icosa_vertices[index3+1], v_icosa_vertices[index3+2]);
		glEnd();
	}
}

void draw_vrml_pyramid()
{
	int i;

	glColor3f(1.0f, 1.0f, 0.0f);
	for(i = 0; i < 48; i+=4)
	{	
		glBegin(GL_LINE_LOOP);
		int index1 = v_pyramid_indices[i]*3;
		int index2 = v_pyramid_indices[i+1]*3;
		int index3 = v_pyramid_indices[i+2]*3;
	
		glVertex3f(v_pyramid_vertices[index1], v_pyramid_vertices[index1+1], v_pyramid_vertices[index1+2]);
		glVertex3f(v_pyramid_vertices[index2], v_pyramid_vertices[index2+1], v_pyramid_vertices[index2+2]);
		glVertex3f(v_pyramid_vertices[index3], v_pyramid_vertices[index3+1], v_pyramid_vertices[index3+2]);
		glEnd();
	}
}

void draw_spikey_cube()
{
	 int i;
        glColor3f(1.0f, 1.0f, 0.0f);

        for(i = 0; i < 30; i+=5)
        {
                int index1 = v_cube_indices[i] * 3;
                int index2 = v_cube_indices[i+1] * 3;
                int index3 = v_cube_indices[i+2] * 3;
                int index4 = v_cube_indices[i+3] * 3;
			
		//pick any (I guess the first) three vertices
		GLfloat vertex_a[3] = {v_cube_vertices[index1], v_cube_vertices[index1+1], v_cube_vertices[index1+2]};
		GLfloat vertex_b[3] = {v_cube_vertices[index2], v_cube_vertices[index2+1], v_cube_vertices[index2+2]};
		GLfloat vertex_c[3] = {v_cube_vertices[index3], v_cube_vertices[index3+1], v_cube_vertices[index3+2]};
		GLfloat vertex_d[3] = {v_cube_vertices[index4], v_cube_vertices[index4+1], v_cube_vertices[index4+2]};
		
		GLfloat *vectors[4] = {vertex_a, vertex_b, vertex_c, vertex_d};
//		make_spike(vectors, 4);
	
		GLfloat *vec1 = (GLfloat*)malloc(3);
		GLfloat *vec2 = (GLfloat*)malloc(3);
		
		//vector combine is okay.
		vec1 = combine_vertices(vertex_b, vertex_a, 3, 0);
		printf("vec1: (%f, %f, %f)\n", vec1[0], vec1[1], vec1[2]);
	
		vec2 = combine_vertices(vertex_c, vertex_a, 3, 0);
		printf("vec2: (%f, %f, %f)\n", vec2[0], vec2[1], vec2[2]);
	
		GLfloat *vertex_cross_product = cross(*vec1, *vec2);
		//printf("cross product vector: (%f, %f, %f)\n", vertex_cross_product[0], vertex_cross_product[1], vertex_cross_product[2]);

		GLfloat *normalized = normalize(vertex_cross_product);	
	//	printf("normalized vector: (%f, %f, %f)\n", normalized[0], normalized[1], normalized[2]);

		GLfloat *center = centerpoint(vectors, 4);
	//	printf("centerpoint: (%f, %f, %f)\n", center[0], center[1], center[2]);

		GLfloat *result = combine_vertices(center, normalized, 3, 1);

		glBegin(GL_LINE_LOOP);
		glVertex3f(result[0], result[1], result[2]);
		glVertex3f(center[0], center[1], center[2]);
		glEnd();

		glBegin(GL_LINE_LOOP);
                glVertex3f(v_cube_vertices[index1], v_cube_vertices[index1+1], v_cube_vertices[index1+2]);
                glVertex3f(v_cube_vertices[index2], v_cube_vertices[index2+1], v_cube_vertices[index2+2]);
                glVertex3f(v_cube_vertices[index3], v_cube_vertices[index3+1], v_cube_vertices[index3+2]);
                glVertex3f(v_cube_vertices[index4], v_cube_vertices[index4+1], v_cube_vertices[index4+2]);
                glEnd();
        }
}

void make_spike(GLfloat **vectors, int num)
{
	GLfloat *center = centerpoint(vectors, num);
	GLfloat *normal = combine_vertices(center,normalize(cross(combine_vertices(vectors[1], vectors[0],3, 0),
                                        combine_vertices(vectors[2], vectors[0],3, 0))) , 3, 1);

	glBegin(GL_LINE_LOOP);
	glVertex3f(normal[0], normal[1], normal[2]);
	glVertex3f(center[0], center[1], center[2]);
	glEnd();
}

GLfloat* normalize(GLfloat* vector)
{
	GLfloat result[3];
	GLfloat length = sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) * (vector[2]*vector[2]));
	
	result[0] = vector[0]/length;
	result[1] = vector[1]/length;
	result[2] = vector[2]/length;

	return result;
}

GLfloat* centerpoint(GLfloat** vectors, int num)
{
	int i = 0;
	GLfloat result[3];
	for(; i < num; i++)
	{
		GLfloat *temp = vectors[i];
		result[0] += temp[0];
		result[1] += temp[1];
		result[2] += temp[2];		
	}
	result[0]/=num;
	result[1]/=num;
	result[2]/=num;
	
	return result;
}

GLfloat* cross(GLfloat* a, GLfloat* b)
{
	GLfloat result[3];
	
	printf("a X b: (%f, %f, %f) x (%f, %f, %f)\n", a[0], a[1], a[2], b[0], b[1], b[2]);
	GLfloat num1 = a[1]*b[2];
	GLfloat num2 = a[2]*b[1];
	printf("%f*%f - %f*%f = %f - %f = %f\n",a[1], b[2], a[2], b[1], num1, num2, num1-num2);

	num1 = a[2]*b[0];
	num2 = a[0]*b[2];
	 printf("%f*%f - %f*%f = %f - %f = %f\n",a[2], b[0], a[0], b[2], num1, num2, num1-num2);
	
	num1 = a[0]*b[1];
	num2 = a[1]*b[0];
	 printf("%f*%f - %f*%f = %f - %f = %f\n",a[0], b[1], a[1], b[0], num1, num2, num1-num2);

	result[0] = ((a[1]*b[2]) - (a[2]*b[1]));
	result[1] = ((a[2]*b[0]) - (a[0]*b[2]));
	result[2] = ((a[0]*b[1]) - (a[1]*b[0]));
	return result;
}

GLfloat* combine_vertices(GLfloat* a, GLfloat* b, int length, int is_add)
{
	GLfloat new_vertices[length];
	int i;
	for(i = 0; i < length; i++)
	{
		if(is_add)
			new_vertices[i] = a[i] + b[i];
		else
			new_vertices[i] = a[i] - b[i];
	}
	
	return new_vertices;
}
