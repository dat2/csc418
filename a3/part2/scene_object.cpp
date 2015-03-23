/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "scene_object.h"

bool updateRay(Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld, double t, Vector3D norm,
    Point3D intersection) {
  if(ray.intersection.none || t < ray.intersection.t_value) {
    ray.intersection.t_value = t;
    ray.intersection.point = modelToWorld * intersection;
    ray.intersection.normal = transNorm(worldToModel, norm);
    ray.intersection.normal.normalize();
    ray.intersection.none = false;

    return true;
  }
  return false;
}

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0),
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.

	// Apply Hint: transform ray from world to object space
	Ray3D mRay;
	mRay.origin = worldToModel * ray.origin;
	mRay.dir = worldToModel * ray.dir;

	// t*dz + oz = 0  --> t = -oz/dz
	double t = -(mRay.origin[2] / mRay.dir[2]);

	// return if t is in the reverse direction of the ray
	if (t <= 0)
		return false;

	// Find x,y co-ordinates of the intersection point
	Point3D intersection = mRay.origin + t * mRay.dir;
	double x = intersection[0];
	double y = intersection[1];

	// Determine whether the intersection lies within the square
	if (x <= 0.5 && x >=-0.5 && y <= 0.5 && y >=-0.5) {
		Vector3D norm(0, 0, 1);

		return updateRay(ray, worldToModel, modelToWorld, t, norm, intersection);
	}

	return false;
}

bool solveQuadraticEquation(double a, double b, double c, double *x) {
  // Calculate discriminant
  double discr = b * b - 4 * a * c;

  // no solution
  if (discr < 0)
    return false;
  else if (discr == 0)
    *x = -b / (2 * a);
  else {
    double t1 = (-b + sqrt(discr) ) / (2 * a);
    double t2 = (-b - sqrt(discr) ) / (2 * a);
    *x = fmin(t1, t2);
  }

  return true;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred
	// on the origin.
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.
	// Source: http://en.wikipedia.org/wiki/Line–sphere_intersection

	// Apply Hint: transform ray from world to object space
	Ray3D mRay;
	mRay.origin = worldToModel * ray.origin;
	mRay.dir = worldToModel * ray.dir;

	// Sphere's center
	Point3D center(0, 0, 0);
	Vector3D oc = mRay.origin - center;

	// Calculate quadratic equation's coefficients (see source)
	double a = mRay.dir.squaredLength();
	double b = 2 * mRay.dir.dot(oc);
	double c = oc.squaredLength() - 1;

  double t;
  if(!solveQuadraticEquation(a, b, c , &t)) {
    return false;
  }

	// return if t is in the reverse direction of the ray
	if (t <= 0)
		return false;

	// Find x,y co-ordinates of the intersection point
	Point3D intersection = mRay.origin + t * mRay.dir;

	// Determine whether the intersection lies within the square
	// Relax the t_value
	Vector3D norm = intersection - center;

	// update
  return updateRay(ray, worldToModel, modelToWorld, t, norm, intersection);
}

// the circle is in the XY plane, and has radius 1
bool intersectsXYCircle(Ray3D& ray, double circleZ, double *t, Point3D *intersection) {
  *t = (circleZ - ray.origin[2]) / (ray.dir[2]);

  // if t is behind the view plane, return false
  if(t <= 0)
    return false;

  // Find x,y co-ordinates of the intersection point
  *intersection = ray.origin + (*t) * ray.dir;

  Vector3D xy ((*intersection)[0], (*intersection)[1], 0);
  double intersectionRadius = xy.squaredLength();

  // we do not need to check t, since we've designed t to set the intersection's
  // z coordinate equal to the circle's z coordinate
  // however we need to check that the intersection point is within the circle
  return intersectionRadius <= 1;
}

bool UnitCylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld ) {

  Ray3D mRay;
  mRay.origin = worldToModel * ray.origin;
  mRay.dir = worldToModel * ray.dir;

  // (1) Cylinder Body
  Point3D origin(0,0,0);

  // helper vectors, to make calculation easier
  Vector3D o = mRay.origin - origin;
  o[2] = 0;
  Vector3D d = mRay.dir;
  d[2] = 0;

  // Calculate quadratic equation's coefficients (see source)
  double a = d.squaredLength();
  double b = 2 * d.dot(o);
  double c = o.squaredLength() - 1;

  double t = 0;

  bool solutionFound = solveQuadraticEquation(a, b, c, &t);
  if(solutionFound) {
    // return false if behind the view plane
    if (t <= 0)
      return false;

    // Find x,y co-ordinates of the intersection point
    Point3D intersection = mRay.origin + t * mRay.dir;

    // Determine whether the intersection lies within the square
    if (intersection[2] <= 1 && intersection[2] >= 0) {
      Vector3D norm(intersection[0], intersection[1], 0);

      return updateRay(ray, worldToModel, modelToWorld, t, norm, intersection);
    }
  }

  // (2) and (3) top and bottom of cylinder
  Point3D intersection;
  bool intersectsTop = intersectsXYCircle(mRay, 1, &t, &intersection);
  if(intersectsTop) {
    Vector3D norm(0, 0, 1);
    return updateRay(ray, worldToModel, modelToWorld, t, norm, intersection);
  }

  bool intersectsBottom = intersectsXYCircle(mRay, 0, &t, &intersection);
  if(intersectsBottom) {
    Vector3D norm(0, 0, -1);
    return updateRay(ray, worldToModel, modelToWorld, t, norm, intersection);
  }

  return false;
}

// parse an OBJ file
Mesh::Mesh(const char *filename) {
  std::ifstream file;
  file.open(filename, std::ios::in);

  if(file.is_open()) {
    std::string line;
    // parse the files
    while (std::getline(file, line))
    {
      // skip comments
      if(line[0] == '#' || line[0] == '\n')
        continue;

      bool parsed = false;

      // parse faces
      if(line[0] == 'f')
        parsed = parseFace(line);

      // ignore texture coordinates
      if(!parsed && line[0] == 'v' && line[1] == 't')
        continue;

      // ignore normals
      if(!parsed && line[0] == 'v' && line[1] == 'n')
        continue;

      // finally, parse vertices
      if(!parsed && line[0] == 'v')
        parsed = parseVertex(line);
    }
  }
  file.close();
}

bool Mesh::parseVertex(std::string& line) {
  // skip the first character, since we already know what it is
  std::istringstream iss(line);
  iss.seekg(1);

  double x,y,z;
  // must have an x,y,z
  if(!(iss >> x >> y >> z)) {
    return false;
  }

  // try to parse optional w
  Point3D parsed(x,y,z);

  vertices.push_back(parsed);

  return true;
}

bool Mesh::parseFace(std::string& line) {
  if(line[0] != 'f')
    return false;

  // skip the first character
  std::istringstream iss(line);
  iss.seekg(1);

  std::vector<int> face;
  int vertexIndex, empty;

  while(iss >> vertexIndex) {
    face.push_back(vertexIndex - 1);

    // 47 is forward slash
    // ignore texture coordinate
    if(iss.peek() == 47) {
      iss.get();
      iss >> empty;
    }
    // ignore normal coordinate
    if(iss.peek() == 47) {
      iss.get();
      iss >> empty;
    }
  }

  faces.push_back(face);

  return true;
}

bool Mesh::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld ) {

  Ray3D mRay;
  mRay.origin = worldToModel * ray.origin;
  mRay.dir = worldToModel * ray.dir;

  bool updated = false;

  // calculate the normal for each face,
  // assume each face is a triangle only for simplicity
  for( auto &face : faces ) {
    Point3D a = vertices[face[0]];
    Point3D b = vertices[face[1]];
    Point3D c = vertices[face[2]];

    Vector3D one = b - a;
    Vector3D two = c - a;

    Vector3D norm = one.cross(two);

    double num = (a - mRay.origin).dot(norm);
    double den = (mRay.dir.dot(norm));

    if(den == 0) {
      continue; // ray and face are parallel
    }

    double t = num / den;
    Point3D intersection = mRay.origin + t * mRay.dir;

    // check that the intersection point is on the plane, inside the face
    // using the algorithm on A1
    norm.normalize();
    int len = face.size();

    bool insideFace = true;
    for( int i = 0; i < len && insideFace; i++) {
      Point3D v_1 = vertices[face[i]];
      Point3D v_2 = vertices[face[(i+1) % len]];

      Vector3D v = v_2 - v_1;
      v.normalize();

      // normal cross v, ensures we get a vector that is perpendicular
      // to the line segment from v_1 to v_2, and is inside the plane,
      // and is also pointing towards the inside of the face
      insideFace = norm.cross(v).dot(intersection - v_1) >= 0;
    }

    if(insideFace && t > 0) {
      updated = updateRay(ray, worldToModel, modelToWorld, t, norm, intersection)
        || updated;
    }
  }

  return updated;
}
