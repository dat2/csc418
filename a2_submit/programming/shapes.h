#ifndef __SHAPES_H_
#define __SHAPES_H_

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#endif

#include "vector.h"

// copy rubiks cube
#define RGB(r,g,b) r/255.f,g/255.f,b/255.f
// html colours
#define RIGHT_COLOUR RGB(139,0,0) // darkred
#define LEFT_COLOUR RGB(255,140,0) // darkorange
#define FRONT_COLOUR RGB(0,0,139) // darkblue
#define BACK_COLOUR RGB(0,139,0) // darkgreen
#define TOP_COLOUR RGB(255,255,255) // white
#define BOTTOM_COLOUR RGB(255,215,0)

class Frustrum
{
public:
  Frustrum(float top_percentage, float height);

  void draw() const;
private:
  Vector points[8];

  Vector normals[6];
};

void setNorm(Vector v);
void setVert(Vector v);
void drawCube(void);

#endif
