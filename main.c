#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "point.h"
#include "spin.h"
#include "mouse.h"
#include "bgcolor.h"
#include "poly.h"

static const int WIDTH = 400;
static const int HEIGHT = 400;

int _window;

spin _spin = { 0, SPIN_DIRECTION_LEFT, 0, false };

mouse _mouse = { { 0, 0 }, false, false };

bgColor _bgColor = { 0.5, 1.0, 1.0, 1.0 };

poly _poly = { MODE_POINTS, WIDTH/2, HEIGHT/2 };

void init( void )
{
	glPointSize( 6.0 );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glShadeModel( GL_FLAT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void equilateralTriangle( int edgeLength )
{
	int height = (sqrt( 3 ) / 2) * edgeLength;

	point p1 = { -(edgeLength / 2), (height / 2) };
	point p2 = { (edgeLength / 2), (height / 2) };
	point p3 = { p1.x + (edgeLength * cos( M_PI / 3 )), p1.y
			- (edgeLength * sin( M_PI / 3 )) };

	if ( _poly.mode == MODE_FILLED )
	{
		glBegin( GL_POLYGON );
		glVertex3i( p1.x, p1.y, 0 );
		glVertex3i( p2.x, p2.y, 0 );
		glVertex3i( p3.x, p3.y, 0 );
		glEnd();
	} else if ( _poly.mode == MODE_POINTS )
	{
		glBegin( GL_POINTS );
		glVertex2i( p1.x, p1.y );
		glVertex2i( p2.x, p2.y );
		glVertex2i( p3.x, p3.y );
		glEnd();
	} else if ( _poly.mode == MODE_LINES )
	{
		glBegin( GL_LINES );
		glVertex2i( p1.x, p1.y );
		glVertex2i( p2.x, p2.y );
		glVertex2i( p2.x, p2.y );
		glVertex2i( p3.x, p3.y );
		glVertex2i( p3.x, p3.y );
		glVertex2i( p1.x, p1.y );
		glEnd();
	}
}

void onDisplay( void )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();

	glColor4f( _bgColor.red, _bgColor.green, _bgColor.blue,
			_bgColor.intensity );
	glRecti( -WIDTH * 2, -HEIGHT * 2, WIDTH * 2, HEIGHT * 2 );

	glColor4f( 1.0, 0.0, 0.0, 1.0 );
	glTranslatef( _poly.x, _poly.y, 0.0 );
	glRotatef( _spin.currentAngle, 0.0, 0.0, 1.0 );
	equilateralTriangle( 200 );
	glTranslatef( -_poly.x, -_poly.y, 0.0 );

	glPopMatrix();
	glutSwapBuffers();
}

void onSpinDisplay( void )
{
	if ( _spin.currentDirection == SPIN_DIRECTION_LEFT )
		_spin.currentAngle -= _spin.speed;
	else
		_spin.currentAngle += _spin.speed;

	if ( _spin.currentAngle > 360.0 )
		_spin.currentAngle -= 360.0;

	if ( _spin.currentAngle < 0.0 )
		_spin.currentAngle += 360.0;

}

void onIdle( void )
{
	bool change = false;
	if ( _spin.isSpinning )
	{
		change = true;
		onSpinDisplay();
		glutPostRedisplay();
	}
}

void onReshape( int w, int h )
{
	glViewport( 0, 0, (GLsizei) w, (GLsizei) h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0f, WIDTH, 0, HEIGHT, 0.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

/**
 * Callback that is registered to GLUT to handle all
 *  keyboard events for this program
 */
void onKeyPress( unsigned char key, int x, int y )
{
	switch ((char) key) {
	case 'q':
	case 27: // esc key
		glutDestroyWindow( _window );
		exit( 0 );
	case 'c':
		_bgColor.red = 0.0;
		_bgColor.green = 1.0;
		_bgColor.blue = 1.0;
		break;
	case 'm':
		_bgColor.red = 1.0;
		_bgColor.green = 0.0;
		_bgColor.blue = 1.0;
		break;
	case 'y':
		_bgColor.red = 1.0;
		_bgColor.green = 1.0;
		_bgColor.blue = 0.0;
		break;
	case 'w':
		_bgColor.red = 1.0;
		_bgColor.green = 1.0;
		_bgColor.blue = 1.0;
		break;
	case '1':
		_poly.mode = MODE_POINTS;
		break;
	case '2':
		_poly.mode = MODE_LINES;
		break;
	case '3':
		_poly.mode = MODE_FILLED;
		break;
	default:
		break;
	}
	glutPostRedisplay();
	return;
}

void onMouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN )
	{
		if ( button == GLUT_MIDDLE_BUTTON )
		{
			_mouse.isMiddlePressed = true;
		} else
		{
			if ( !_mouse.isLeftRightPressed )
			{
				_spin.speed = DEFAULT_SPIN_SPEED;
				_mouse.home.x = x;
				_mouse.home.y = y;
				_mouse.isLeftRightPressed = true;
				_bgColor.intensity = 0.5;
			}

			if ( button == GLUT_LEFT_BUTTON )
				_spin.currentDirection = SPIN_DIRECTION_LEFT;
			if ( button == GLUT_RIGHT_BUTTON )
				_spin.currentDirection = SPIN_DIRECTION_RIGHT;
			_spin.isSpinning = true;
		}
	} else if ( state == GLUT_UP )
	{
		if ( button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON )
			_spin.isSpinning = false;

		_mouse.isLeftRightPressed = false;
		_mouse.isMiddlePressed = false;
	}
}

void onMouseMove( int x, int y )
{
	if ( _mouse.isLeftRightPressed )
	{
		int yDelta = _mouse.home.y - y;

		if ( yDelta > 0 ) 	// speed up spin
		{
			GLfloat speed = (y * DEFAULT_SPIN_SPEED) / _mouse.home.y;
			speed = DEFAULT_SPIN_SPEED - speed;
			_spin.speed = DEFAULT_SPIN_SPEED + speed;
		} else
		{ 					// slow down spin
			GLfloat speed = (y - _mouse.home.y)
					* (DEFAULT_SPIN_SPEED / (HEIGHT - _mouse.home.y));
			_spin.speed = DEFAULT_SPIN_SPEED - speed;
		}

		int xDelta = _mouse.home.x - x;

		if ( xDelta < 0 )	// increase intensity
		{
			GLfloat intensity = ((x - _mouse.home.x) * 0.5)
					/ (WIDTH - _mouse.home.x);
			_bgColor.intensity = 0.5 + intensity;
		} else				// decrease intensity
		{
			_bgColor.intensity = (x * 0.5) / _mouse.home.x;
		}
	}
}

/*
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( WIDTH, HEIGHT );
	glutInitWindowPosition( 100, 100 );
	_window = glutCreateWindow( "Robert Timm - Assignment 2" );
	init();
	glutDisplayFunc( onDisplay );
	glutIdleFunc( onIdle );
	glutReshapeFunc( onReshape );
	glutMouseFunc( onMouse );
	glutKeyboardFunc( onKeyPress );
	glutMotionFunc( onMouseMove );
	glutMainLoop();
	return 0;
}
