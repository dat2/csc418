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
  points[0][1] = points[4][1];

  points[1] = points[5] * top_percentage;
  points[1][1] = points[5][1];

  points[2] = points[6] * top_percentage;
  points[2][1] = points[6][1];

  points[3] = points[7] * top_percentage;
  points[3][1] = points[7][1];

  for(int i = 0; i < 4; i++) {
    points[i][1] += height;
  }

  // front
  normals[0][2] = -1.f;

  // back
  normals[1][2] = 1.f;

  // left
  normals[2][0] = 1.f;

  // right
  normals[3][0] = -1.f;

  // top
  normals[4][1] = -1.f;
  // bottom
  normals[5][1] = 1.f;
}

void setNorm(Vector v) {
  glNormal3f(v[0], v[1], v[2]);
}

void setVert(Vector v) {
  glVertex3f(v[0], v[1], v[2]);
}

// i defined the points all weird so drawing is weird
void Frustrum::draw() const {
  glBegin(GL_QUADS);
    // draw front face
    // glColor3f(FRONT_COLOUR);
    setNorm(normals[0]);
    setVert(points[4]);
    setVert(points[7]);
    setVert(points[3]);
    setVert(points[0]);

    // draw back face
    // glColor3f(BACK_COLOUR);
    setNorm(normals[1]);
    setVert(points[6]);
    setVert(points[5]);
    setVert(points[1]);
    setVert(points[2]);

    // draw left face
    // glColor3f(LEFT_COLOUR);
    setNorm(normals[2]);
    setVert(points[4]);
    setVert(points[0]);
    setVert(points[1]);
    setVert(points[5]);

    // draw right face
    // glColor3f(RIGHT_COLOUR);
    setNorm(normals[3]);
    setVert(points[7]);
    setVert(points[6]);
    setVert(points[2]);
    setVert(points[3]);

    // draw top
    // glColor3f(TOP_COLOUR);
    setNorm(normals[4]);
    setVert(points[0]);
    setVert(points[3]);
    setVert(points[2]);
    setVert(points[1]);

    // draw bottom
    // glColor3f(BOTTOM_COLOUR);
    setNorm(normals[5]);
    setVert(points[4]);
    setVert(points[5]);
    setVert(points[6]);
    setVert(points[7]);
  glEnd();
}
