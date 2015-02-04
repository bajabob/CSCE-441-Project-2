#ifndef MOUSE_H_
#define MOUSE_H_
#include "point.h"

typedef struct mouse mouse;
struct mouse {
   point home;
   bool isLeftRightPressed;
   bool isMiddlePressed;
};

#endif
