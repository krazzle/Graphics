/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "drawing.h"
#include "data.h"
//#include "mouse.h" 
#include "helper.cpp"


#define MOUSE_ROTATE_YX                0
#define MOUSE_ROTATE_YZ                1
#define LIGHT_MOVE                     2
#define MOUSE_ZOOM             3
#define GOURAD 				1
#define PHONG				2


/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;                /* window width */
int H=400;                /* window height */
int X_OFF = 10;        /* window x offset */
int Y_OFF = 10;        /* window y offset */

int drawStyleState = 0;
int faceOrPoints = 0;
/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;
GLfloat zoomFactor = 1.0;
int mouse_mode = 0;
int m_last_x = 0;
int m_last_y = 0;
int shading_style = GOURAD;

GLfloat cpts[30*64][3*64][3];
GLfloat normals[30*64][3*64][3];
int ncpts = 0;
int totalPoints = ncpts;
int threeDmode = 0;
int numOfVerticalSubs = 0;
int numOfHorizontalSubs = 0;
int offset = 64;
int hOffset = 64;
int hCount = 3;

GLfloat pos[] = {0, 0, -10};


/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void myMouseMotion(int x, int y);
void endSubdiv(int status);
void displayPointsAndLines();
void displayRotatedPointsAndLines();
void phil(GLfloat theta, int horiz_loc);
void VerticalSubdivide();
void HorizontalSubdivide();
void calculateNormals();
void resetAll();

void getNormal(GLfloat* unitNormal, GLfloat* a, GLfloat* b, GLfloat* c);
void crossProduct(GLfloat* result ,GLfloat* a, GLfloat*b);


int main (int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(X_OFF, Y_OFF);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("subdiv");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyHandler);
    glutMouseFunc(myMouseButton);
    glutMotionFunc(myMouseMotion);
    glutMainLoop();
    return 0;
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
    hCount = 3;
    hOffset = 64;        
    //        int i, j;
    //        for(i = 0; i < 960; i++)
    //                for(j = 0; j < 3; j++) 
    //                        cpts[i][j][0] = -99;
}

void display() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
     * See drawing.c for the definition of these routines.
     *
     * Note: Only one should be called at a time (based on the
     * display mode).
     */
    if (threeDmode)
        displayRotatedPointsAndLines();
    else
        displayPointsAndLines();
    drawSurface();

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
    switch(ch) {
        case 'q':
            endSubdiv(0);
            break;
        case 'w': 
            //  if ( ncpts >= 5) {
            threeDmode = 1;
            phil(120, 64);
            phil(-120, 128);
            calculateNormals();
            // } else
            //   printf("include at least five points\n");
            break;
        case 'e':
            if ( drawStyleState == 0)
                drawStyleState = 1;
            else  
                drawStyleState = 0;
            display();
            break;
	case 'd':
	    if(shading_style == GOURAD)
		shading_style = PHONG;
	    else
		shading_style = GOURAD;
	    display();
	    break;
        case 'r':
            if ( faceOrPoints == 0)
                faceOrPoints = 1;
            else
                faceOrPoints = 0;
            display();
            break;
        case 'a':
            VerticalSubdivide();
            display();
            break;
        case 'b':
            HorizontalSubdivide();
            display();
            break;
        case 'z':
            threeDmode = 0;
            resetAll();
        default:
            /* Unrecognized keypress */
            return;
    }

    glutPostRedisplay(); 
    return;
}

void resetAll(){
    //ncpts = 0;
    totalPoints = ncpts;
    threeDmode = 0;
    numOfVerticalSubs = 1;
    numOfHorizontalSubs = 1;
    offset = 64;
    hOffset = 64;
    hCount = 3;
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
    display();
}

void HorizontalSubdivide(){
    if(numOfHorizontalSubs == 6)
        return;
    numOfHorizontalSubs++;

    int oldHOffset = hOffset;
    hOffset /= 2;
    hCount *= 2;
    GLfloat thetaOffset = 360/(GLfloat)hCount;
    GLfloat theta = thetaOffset;

    int i;
    for ( i = hOffset; i < hCount*hOffset; i += oldHOffset) {
        //printf("filling location %d with angle %f\n", i, theta);
        phil(theta, i);
        theta += thetaOffset*2;
    }
    calculateNormals();
}

void VerticalSubdivide(){
    if(numOfVerticalSubs == 6)
        return;
    numOfVerticalSubs++;
    int old_offset = offset;
    offset/=2;
    totalPoints = (2*totalPoints -1);
    int i;
    for(i = 0;i < totalPoints*offset; i+=offset){
        //if((i +old_offset) >= ncpts*32)
        //        continue;
        //printf("i: %d offset: %d old_offset: %d\n", i, offset, old_offset);        
        if((i == 0) | ((i + offset)>= totalPoints*offset))
            continue;
        if(i%old_offset == 0){
            //        printf("replacing new point %d\n", i);
            //updating old points
            GLfloat p0[2] = {cpts[i - old_offset][0][0], cpts[i - old_offset][0][1]};
            GLfloat p1[2] = {cpts[i][0][0], cpts[i][0][1]};
            GLfloat p2[2] = {cpts[i+old_offset][0][0], cpts[i+old_offset][0][1]};
            cpts[i][0][0] = (p0[0] + (6*p1[0]) + p2[0])/8;
            cpts[i][0][1] = (p0[1] + (6*p1[1]) + p2[1])/8;
        }
        else{
            //adding new points
            //        printf("adding new point %d\n", i);
            GLfloat p0[2] = {cpts[i - offset][0][0], cpts[i-offset][0][1]};
            GLfloat p1[2] = {cpts[i + offset][0][0], cpts[i + offset][0][1]};
            cpts[i][0][0] = ((4*p0[0]) + (4*p1[0]))/8;
            cpts[i][0][1] = ((4*p0[1]) + (4*p1[1]))/8;        
        }

    }

    //        printf("for new added points, adding %d elements in radial direction\n", hCount);
    GLfloat thetaOffset = 360/(GLfloat)hCount;        
    GLfloat theta = thetaOffset;
    for(i = hOffset; i < hCount*hOffset; i+=hOffset){
        phil(theta, i);
        theta += thetaOffset;
    }
    calculateNormals();
    //totalPoints = (2*totalPoints -1);
}


void phil(GLfloat theta, int horiz_loc ) {

    GLfloat degreesToRads = (1*3.141592654)/180;
    GLfloat trad = degreesToRads * theta;

    // rotation matrix for 120 degrees
    GLfloat m11[4] = { cos(trad), 0, sin(trad), 0};
    GLfloat m12[4] = { 0, 1, 0, 0};
    GLfloat m13[4] = { -1*sin(trad), 0, cos(trad), 0};
    GLfloat m14[4] = { 0, 0, 0, 1};
    GLfloat *m1[4] = { m11, m12, m13, m14};

    // rotation matrix for -120 degrees
    GLfloat v1[1] = {0};
    GLfloat v2[1] = {0};
    GLfloat v3[1] = {0};
    GLfloat v4[1] = {1};
    GLfloat *v[4] = { v1, v2, v3, v4};

    int i, j;
    for ( i = 0; i < ncpts*64; i += offset) { 
        // get point
        v[0][0] = cpts[i][0][0];
        v[1][0] = cpts[i][0][1];
        v[2][0] = cpts[i][0][2];
        v[3][0] = 1;

        // rotate by m1, which is 120 degrees
        GLfloat** result = multiply(4, 4, m1, 4, 1, v);

        // put it back in there after rotated
        cpts[i][horiz_loc][0] = result[0][0];
        cpts[i][horiz_loc][1] = result[1][0];
        cpts[i][horiz_loc][2] = result[2][0];
    }
    display();
}

void displayPointsAndLines(){
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0, -40, 0);
    glVertex3f(0, 40, 0);
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glPointSize(5);
    int i;
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < ncpts*64; i+=64) {
        //  printf("%f %f\n", cpts[i][0], cpts[i][1]);
        glVertex3fv(cpts[i][0]);
    }
    glEnd();

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POINTS);
    for (i = 0; i < ncpts*64; i+=64)
        glVertex3fv(cpts[i][0]);
    glEnd();
}

void calculateNormals(){
    int i,j;
  //  GLfloat temp_normals[30*64][3*64][3];
    for( i = 0; i < totalPoints*offset; i+=offset){
        for(j = 0; j < hCount*hOffset; j += hOffset){
            if((j+hOffset) <  hCount*hOffset){
                getNormal((GLfloat*)normals[i][j], (GLfloat*)(cpts[i][j]), (GLfloat*)(cpts[i][j+hOffset]), (GLfloat*)(cpts[i+offset][j+hOffset]));    
	    } else {
                getNormal((GLfloat*)normals[i][j], (GLfloat*)(cpts[i][j]), (GLfloat*)(cpts[i][0]), (GLfloat*)(cpts[i+offset][0])); 
            }
        }
    }

/*    for( i = 0; i < totalPoints*offset; i+= offset){
	for(j = 0; j<hCount*hOffset;j+=hOffset){
	    if(i == top & j == top ){
		normals[i][j] = temp_normals[i][j];
	    } else if(i == top & j != top){
		normals[i][j][0] = (temp_normals[i][j][0]+temp_normals[i][j+hOffset][0])/2;
		normals[i][j][1] = (temp_normals[i][j][1]+temp_normals[i][j+hOffset][1])/2;
		normals[i][j][2] = (temp_normals[i][j][2]+temp_normals[i][j+hOffset][2])/2;	
	    } else if(j == top & i != top){
		normals
	    }
	}
    }	
  */  

}

void drawTriangles(GLfloat *point1, GLfloat *point2, GLfloat* point3){
	GLfloat mid1_2[3];
	GLfloat mid2_3[3];
	GLfloat mid3_1[3];
	mid1_2[0] = (point1[0]+point2[0])/2;
	mid1_2[1] = (point1[1]+point2[1])/2;
	mid1_2[2] = (point1[2]+point2[2])/2;
	mid2_3[0] = (point2[0]+point3[0])/2;
	mid2_3[1] = (point2[1]+point3[1])/2;
	mid2_3[2] = (point2[2]+point3[2])/2;
	mid3_1[0] = (point3[0]+point1[0])/2;
	mid3_1[1] = (point3[1]+point1[1])/2;
	mid3_1[2] = (point3[2]+point1[2])/2;
	
	GLfloat normal[3];
	
	//triangle 1
	getNormal((GLfloat*)normal, (GLfloat *)mid3_1, (GLfloat*)mid1_2, point1);
	glBegin(GL_POLYGON);
	glNormal3f(normal[0], normal[1], normal[2]);
	glVertex3fv(mid3_1);
	glVertex3fv(mid1_2);
	glVertex3fv(point1);
	glEnd();
	
	//triangle 2
	getNormal((GLfloat*)normal, point3, (GLfloat*)mid2_3, (GLfloat*)mid3_1);
	glBegin(GL_POLYGON);
	glNormal3f(normal[0], normal[1], normal[2]);
	glVertex3fv(point2);
	glVertex3fv(mid2_3);
	glVertex3fv(mid3_1);
	glEnd();
	
	//triangle 3
	getNormal((GLfloat*)normal, (GLfloat*)mid3_1, (GLfloat*)mid2_3, (GLfloat*)mid1_2);
	glBegin(GL_POLYGON);
	glNormal3f(normal[0], normal[1], normal[2]);
	glVertex3fv(mid3_1);
	glVertex3fv(mid2_3);
	glVertex3fv(mid1_2);
	glEnd();
	
	//triangle 4
	getNormal((GLfloat*)normal, (GLfloat*)mid2_3, point2, (GLfloat*)mid1_2);
	glBegin(GL_POLYGON);
	glNormal3f(normal[0], normal[1], normal[2]);
	glVertex3fv(mid2_3);
	glVertex3fv(point2);
	glVertex3fv(mid1_2);
	glEnd();
	
}

void displayRotatedPointsAndLines(){
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0, -40, 0);
    glVertex3f(0, 40, 0);
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glPointSize(5);
    int i,j;        

    if ( drawStyleState == 0 && faceOrPoints == 0) {

        // lighting!
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        // position!
        //GLfloat pos[] = {0, 0, -10};
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        // colors (get rid of one)
        GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0};
        GLfloat blueAmbientLight[] = {0, 0, 1.0};
        GLfloat whiteSpecularLight[] = {0, 0, 1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
        glLightfv(GL_LIGHT0, GL_AMBIENT, blueAmbientLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
        // colors (get rid of one)
        GLfloat params[3];
        params[0] = .5;
        params[1] = .5;
        params[2] = .5;
        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, params);
        // material!

        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
        // wut
        glEnable(GL_NORMALIZE);

        for( i = 0; i < totalPoints*offset; i+=offset){
            for(j = 0; j < hCount*hOffset; j += hOffset){
                //if((i + offset) >= totalPoints*offset)
                //      continue;
                if((j+hOffset) <  hCount*hOffset){
                    //printf("drawing polygon starting at i = %d j = %d\n", i,j);
                    if(shading_style == GOURAD){
 		    	glBegin(GL_POLYGON);
                    //   GLfloat normal[3];
                    //  getNormal((GLfloat*)normal, (GLfloat*)(cpts[i][j]), (GLfloat*)(cpts[i][j+hOffset]), (GLfloat*)(cpts[i+offset][j+hOffset]));
                    //  glNormal3f(normal[0], normal[1], normal[2]);
                    	glNormal3fv(normals[i][j]);
                    	glVertex3fv(cpts[i][j]);
                   // glNormal3fv(normals[i][j+hOffset]);
                    	glVertex3fv(cpts[i][j+hOffset]);
                   // glVertex3fv(normals[i+offset][j+hOffset]);
                    	glVertex3fv(cpts[i+offset][j+hOffset]);
                   // glVertex3fv(normals[i+offset][j]);
                    	glVertex3fv(cpts[i+offset][j]);
                    	glEnd();
		    } else {
			drawTriangles(cpts[i][j], cpts[i+offset][j], cpts[i][j+hOffset]);
			drawTriangles(cpts[i+offset][j], cpts[i+offset][j+hOffset], cpts[i][j+hOffset]);
                    }
		} else {
                    //printf("here as last\n");
                    //printf("drawing finishing! i = %d j = %d; hCount(%d)*hOffset(%d) = %d; j+hOffset = %d\n", i,j, hCount, hOffset, hCount*hOffset,j+hOffset);
                    //  printf("drawing polygon starting at i = %d j = %d\n", i,j);
		    if(shading_style == GOURAD){
                    	glBegin(GL_POLYGON);
                    // GLfloat normal[3];
                    // getNormal((GLfloat*)normal, (GLfloat*)(cpts[i][j]), (GLfloat*)(cpts[i][0]), (GLfloat*)(cpts[i+offset][0]));
                    // glNormal3f(normal[0], normal[1], normal[2]);
                    //printf("making final polygon from P(%d,%d) -> P(%d,%d) -> P(%d,%d) -> P(%d,%d)\n", i, j, i+offset, j, i+offset, 0, i, 0);
                    	glNormal3fv(normals[i][j]);
                    	glVertex3fv(cpts[i][j]);
                   // glVertex3fv(normals[i][0]);
                    	glVertex3fv(cpts[i][0]);
                   // glVertex3fv(normals[i+offset][0]);
                    	glVertex3fv(cpts[i+offset][0]);
                   // glVertex3fv(normals[i+offset][j]);
                    	glVertex3fv(cpts[i+offset][j]);
                    	glEnd();
		    } else {
			drawTriangles(cpts[i][j], cpts[i+offset][j], cpts[i][0]);
			drawTriangles(cpts[i+offset][j], cpts[i+offset][0], cpts[i][0]);

		    }
                }
            }
        }
    }

    //        printf("hCount: %d totalPoints: %d\n", hCount, totalPoints);

    if ( drawStyleState == 1 && faceOrPoints == 0) {
        //                printf("drawing line from: ")i;
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        for(i = 0; i < hCount*hOffset; i+=hOffset){
            glBegin(GL_LINE_STRIP);
            for(j = 0; j < totalPoints*offset;j+=offset){ 
                //                                printf(" cpts[%d][%d]: (%f,%f,%f)\n", j, i, cpts[j][i][0], cpts[j][i][1], cpts[j][i][2]);
                glVertex3fv(cpts[j][i]);
            }
            glEnd();
        }                
        for (i = 0; i < totalPoints*offset; i+=offset) {
            glBegin(GL_LINE_LOOP);
            for (j = 0; j < hCount*hOffset; j+=hOffset){
                glVertex3fv(cpts[i][j]);
            }
            glEnd();
        }
        //                printf("\n");
    }

    if ( drawStyleState == 0 && faceOrPoints == 1 
            || (drawStyleState == 1 && faceOrPoints == 1)) {
        glColor3f(0.0,0.0,1.0);
        glPointSize(1.0);
        //glBegin(GL_POINTS);
        for (i = 0; i < totalPoints*offset; i+=offset) {
            glBegin(GL_POINTS);
            for (j = 0; j < hCount*hOffset; j+=hOffset) {        
                glVertex3fv(cpts[i][j]);
            }
            glEnd();
        }
    }
    //glEnd();
}

void getNormal(GLfloat* unitNormal, GLfloat* a, GLfloat* b, GLfloat* c) {

    GLfloat v1[3];
    GLfloat v2[3];

    v1[0] = a[0] - b[0];
    v1[1] = a[1] - b[1];
    v1[2] = a[2] - b[2];

    v2[0] = a[0] - c[0];
    v2[1] = a[1] - c[1];
    v2[2] = a[2] - c[2];

    GLfloat normal[3];
    crossProduct( (GLfloat*)normal, (GLfloat*)v1, (GLfloat*)v2);

    GLfloat length = sqrt( normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2] );

    unitNormal[0] = normal[0] / length;
    unitNormal[1] = normal[1] / length;
    unitNormal[2] = normal[2] / length;

}

void crossProduct(GLfloat* result ,GLfloat* a, GLfloat*b) {

    result[0] = ((a[1]*b[2]) - (a[2]*b[1]));
    result[1] = ((a[2]*b[0]) - (a[0]*b[2]));
    result[2] = ((a[0]*b[1]) - (a[1]*b[0]));

}

void myMouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if(threeDmode == 0){
            if (button == GLUT_RIGHT_BUTTON) {
                ncpts += -1;
                totalPoints = ncpts;
            } else {
                if((x < ((W/2) + 5)) && (x > ((W/2) - 5)))
                    x = W/2;

                GLfloat wx = (2.0 * x) / (float)(W - 1) - 1.0;
                GLfloat wy = (2.0 * (H - 1 - y)) / (float)(H - 1) - 1.0;

                wx*= fright;
                wy*= ftop;
                if (ncpts == 30)
                    return;
                cpts[ncpts*offset][0][0] = wx;
                cpts[ncpts*offset][0][1] = wy;
                cpts[ncpts*offset][0][2] = 0.0;
                ncpts++;
                totalPoints = ncpts;
            }
            glFlush();
            display();
        }
        else{
            m_last_x = x;
            m_last_y = y;

            if (button == GLUT_LEFT_BUTTON) {
                mouse_mode = MOUSE_ROTATE_YX;
            } else if (button == GLUT_MIDDLE_BUTTON) {
                mouse_mode = MOUSE_ROTATE_YZ;
            } else if (button == GLUT_RIGHT_BUTTON) {
                mouse_mode = LIGHT_MOVE;
            }
        }
    }
}



void endSubdiv(int status) {
    printf("\nQuitting subdivision program.\n\n");
    fflush(stdout);

    exit(status);
}


/* end of subdiv.cpp */


void myMouseMotion(int x, int y) {
    if(threeDmode == 0)
        return;

    double d_x, d_y;        /* The change in x and y since the last callback */

    d_x = x - m_last_x;
    d_y = y - m_last_y;

    m_last_x = x;
    m_last_y = y;

    if (mouse_mode == MOUSE_ROTATE_YX) {
        /* scaling factors */
        d_x /= 2.0;
        d_y /= 2.0;

        glRotatef(d_x, 0.0, 1.0, 0.0);        /* y-axis rotation */
        glRotatef(-d_y, 1.0, 0.0, 0.0);        /* x-axis rotation */

    } else if (mouse_mode == MOUSE_ROTATE_YZ) {
        /* scaling factors */
        d_x /= 2.0;
        d_y /= 2.0;

        glRotatef(d_x, 0.0, 1.0, 0.0);        /* y-axis rotation */
        glRotatef(-d_y, 0.0, 0.0, 1.0);        /* z-axis rotation */

    } else if (mouse_mode == MOUSE_ZOOM) {
        d_y /= 100.0;

        zoomFactor += d_y;

        if (zoomFactor <= 0.0) {
            /* The zoom factor should be positive */
            zoomFactor = 0.001;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /*
         * glFrustum must receive positive values for the near and far
         * clip planes ( arguments 5 and 6 ).
         */
        glFrustum(fleft*zoomFactor, fright*zoomFactor,
                fbottom*zoomFactor, ftop*zoomFactor,
                -zNear, -zFar);
    } else if (mouse_mode == LIGHT_MOVE){
        d_x /=2;
        d_y /=2;
        pos[0] +=d_x;
        pos[1] +=d_y;       
    }
    /* Redraw the screen */
    glutPostRedisplay();
}

