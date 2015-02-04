#ifndef POLY_H_
#define POLY_H_

static const GLshort MODE_POINTS = 1;
static const GLshort MODE_LINES = 2;
static const GLshort MODE_FILLED = 3;

typedef struct poly poly;
struct poly {
   GLshort mode;
   int x, y;
};

#endif
