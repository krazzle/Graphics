

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

  //printf("aaaaa\n");
  
  //GLfloat **result = (GLfloat**)malloc(m1Columns*m2Rows*sizeof(GLfloat));
  GLfloat ** result = new GLfloat*[m1Rows];
  int s;
  for ( s = 0; s < m1Rows; s += 1) 
    result[s] = new GLfloat[m2Columns];
  // check if valid multiplication
  assert(m1Columns == m2Rows);
  
// dem loopz
  int i, x, y;
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

 // printf("bbbbb\n");
  return result;
}



// make a thing to make the matricies
