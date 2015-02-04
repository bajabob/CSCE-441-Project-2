#ifndef SPIN_H_
#define SPIN_H_


static const GLbyte SPIN_DIRECTION_LEFT  = 0;
static const GLbyte SPIN_DIRECTION_RIGHT = 1;

static const GLfloat DEFAULT_SPIN_SPEED = 10.0;

typedef struct spin spin;
struct spin {
   GLfloat currentAngle;
   GLbyte currentDirection;
   GLfloat speed;
   bool isSpinning;
};


#endif
