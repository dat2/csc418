#include "shapes.h"

#include "vector.h"
#include "stdio.h"

#include <assert.h>


Frustrum::Frustrum(float top_percentage, float height)
{
  Vector x;
  x[0] = 2;

  Vector z;
  z[2] = 2;

  // init the bottom
  points[4][0] = -1;
  points[4][1] = -1;
  points[4][2] = 1;

  points[5] = points[4] - z;
  points[6] = points[4] - z + x;
  points[7] = points[4] + x;

  // init the top
  points[0] = points[4] * top_percentage;
  points[1] = points[5] * top_percentage;
  points[2] = points[6] * top_percentage;
  points[3] = points[7] * top_percentage;

  for(int i = 0; i < 4; i++) {
    points[i][1] += height;
  }
}

void vectorToVert(Vector v) {
  glVertex3f(v[0], v[1], v[2]);
}

// i defined the points all weird so drawing is weird
void Frustrum::draw() const {
  glBegin(GL_QUADS);
    // draw front face
    // glColor3f(FRONT_COLOUR);
    vectorToVert(points[4]);
    vectorToVert(points[7]);
    vectorToVert(points[3]);
    vectorToVert(points[0]);

    // draw back face
    // glColor3f(BACK_COLOUR);
    vectorToVert(points[6]);
    vectorToVert(points[5]);
    vectorToVert(points[1]);
    vectorToVert(points[2]);

    // draw left face
    // glColor3f(LEFT_COLOUR);
    vectorToVert(points[4]);
    vectorToVert(points[0]);
    vectorToVert(points[1]);
    vectorToVert(points[5]);

    // draw right face
    // glColor3f(RIGHT_COLOUR);
    vectorToVert(points[7]);
    vectorToVert(points[6]);
    vectorToVert(points[2]);
    vectorToVert(points[3]);

    // draw top
    // glColor3f(TOP_COLOUR);
    vectorToVert(points[0]);
    vectorToVert(points[3]);
    vectorToVert(points[2]);
    vectorToVert(points[1]);

    // draw bottom
    // glColor3f(BOTTOM_COLOUR);
    vectorToVert(points[4]);
    vectorToVert(points[5]);
    vectorToVert(points[6]);
    vectorToVert(points[7]);
  glEnd();
}
