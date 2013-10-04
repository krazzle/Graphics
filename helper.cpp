

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

using namespace std;

GLfloat**  multiply( int m1Rows, int m1Columns, GLfloat **m1, int m2Rows, int m2Columns, GLfloat **m2) { 
  
  // check if valid multiplication
  assert(m1Columns == m2Rows);

  // GLfloat** r = (GLfloat**)malloc(m1Columns*m2Rows);

  GLfloat** r = new GLfloat*[m2Rows];
  int s;
  for(s = 0; s < m2Rows; s += 1) 
    r[s] = new GLfloat[m1Columns];

  
// dem loopz
  int i, x, y;
  for( i = 0; i < m1Rows; i += 1) 
  {
    int size = m1Columns;
    for( x = 0; x < size; x += 1) {
      GLfloat val = 0;
      for( y = 0; y < size; y += 1) {
	//cout << "i: " << i << " y: " << y << " x: " << x << endl; 
//	cout << "*m1: " << *m1 << endl;
	GLfloat num1 = m1[i][y];
	GLfloat num2 = m2[y][x];
//	cout << "val: " << val << endl;
	val += num1*num2;
      }
  //    cout << "yo niggg" << endl;
     // cout << "val: " << val << " i: " << i << " x: " << x << endl;
      r[i][x] = val;
    }
  }


  cout << "exiting multiply..." << endl;
 
  return r;
}


GLfloat** convertToDubz(int rows, int cols, GLfloat *)
{
		
}
