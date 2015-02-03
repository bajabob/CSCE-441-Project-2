#include <stdlib.h>
#include <math.h>

#include "point.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

static GLfloat spin = 0.0;

static int WIDTH = 400;
static int HEIGHT = 400;

void init( void )
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glShadeModel( GL_FLAT );
}

void equilateralTriangle( int edgeLength )
{
	int height = (sqrt( 3 ) / 2) * edgeLength;

	point p1 = { -(edgeLength/2), (height/2) };
	point p2 = { (edgeLength/2), (height/2) };
	point p3 = { p1.x+(edgeLength*cos(M_PI/3)), p1.y-(edgeLength*sin(M_PI/3)) };

	glBegin( GL_POLYGON );
	glVertex3i( p1.x, p1.y, 0 );
	glVertex3i( p2.x, p2.y, 0 );
	glVertex3i( p3.x, p3.y, 0 );
	glEnd();
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();

	glColor3f( 1.0, 1.0, 1.0 );

	glTranslatef(200, 200, 0.0);
	glRotatef( spin, 0.0, 0.0, 1.0 );
	equilateralTriangle( 400 );
	glTranslatef(-200, -200 ,0.0);

	glPopMatrix();
	glutSwapBuffers();
}

void spinDisplay( void )
{
	spin = spin + 10;
	if ( spin > 360.0 )
		spin = spin - 360.0;
	glutPostRedisplay();
}

void reshape( int w, int h )
{
	glViewport( 0, 0, (GLsizei) w, (GLsizei) h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0f, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void mouse( int button, int state, int x, int y )
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if ( state == GLUT_DOWN )
			glutIdleFunc( spinDisplay );
		break;
	case GLUT_MIDDLE_BUTTON:
		if ( state == GLUT_DOWN )
			glutIdleFunc( NULL );
		break;
	default:
		break;
	}
}

/*
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( WIDTH, HEIGHT );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );
	init();
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
	glutMainLoop();
	return 0;
}
