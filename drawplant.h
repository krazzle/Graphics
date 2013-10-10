#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */


void load2DMatrix(
                GLfloat m00, GLfloat m01, GLfloat m02,
                GLfloat m10, GLfloat m11, GLfloat m12,
                GLfloat m20, GLfloat m21, GLfloat m22) ;
void drawPlant(void);
void drawLSystem(char str[], int len);
void printMatrix(GLfloat **mat);

#endif	/* _DRAWPLANT_H_ */
