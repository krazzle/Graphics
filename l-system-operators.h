#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <iostream>

#include "common.h"
#include "drawplant.h"
#include "readppm.h"

#include "helper.h"


GLfloat** translate(GLfloat **mat, GLfloat percent, GLfloat x, GLfloat y, GLfloat z);
GLfloat** pop();
void push(GLfloat **mat);
GLfloat** rotate(GLfloat **mat, GLfloat x, GLfloat y, GLfloat z);
void load2DMatrixWrapper(GLfloat** mat);
void copyMatrix(int rows, int columns, GLfloat** m1, GLfloat** m2);
void load3DMatrixWrapper(GLfloat** mat);
