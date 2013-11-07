

#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <iostream>

#include "common.h"
//#include "drawplant.h"
//#include "readppm.h"

#include "helper.h"

using namespace std;

GLfloat**  multiply( int m1Rows, int m1Columns, GLfloat **m1, int m2Rows, int m2Columns, GLfloat **m2) { 

  GLfloat **result = (GLfloat **)malloc(m1Rows*sizeof(GLfloat*));
  int i;
  for(i = 0; i < m1Rows; i++)
	result[i] = (GLfloat *)malloc(m1Columns*sizeof(GLfloat));
  assert(m1Columns == m2Rows);
  
// dem loopz
  int x, y;
  for( i = 0; i < m1Rows; i += 1) 
  {
    int size = m1Columns;
    for( x = 0; x < size; x += 1) {
      GLfloat val = 0;
      for( y = 0; y < size; y += 1) {
	GLfloat num1 = m1[i][y];
	GLfloat num2 = m2[y][x];
	val += num1*num2;	
      }
      result[i][x] = val;
    }
  }

 
  return result;
}



// make a thing to make the matricies
