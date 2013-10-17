#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */

void load3DMatrix(
                GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
                GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
                GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
                GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33);
void load2DMatrix(
                GLfloat m00, GLfloat m01, GLfloat m02,
                GLfloat m10, GLfloat m11, GLfloat m12,
                GLfloat m20, GLfloat m21, GLfloat m22) ;
void drawPlant(void);
void drawLSystem(char str[], int len);
void printMatrix(GLfloat **mat);

#endif	/* _DRAWPLANT_H_ */
