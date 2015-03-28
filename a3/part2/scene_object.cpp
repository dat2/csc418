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

bool intersectsPlane(Ray3D &ray, double &t, Vector3D &in, Point3D &intersection,
  int axis, int normVal, double p, double aMin, double aMax, double bMin, double bMax) {
  // t*d + o = p  --> t = (p-o)/d
  t = (p - ray.origin[axis] / ray.dir[axis]);

  // return if t is in the reverse direction of the ray
  if (t <= 0)
    return false;

  intersection = ray.origin + t * ray.dir;

  Vector3D norm;
  double a,b;
  switch(axis) {
    case 0:
      a=intersection[1],b=intersection[2];
      norm[0] = normVal;
      norm.normalize();
      break;
    case 1:
      a=intersection[0],b=intersection[2];
      norm[1] = normVal;
      norm.normalize();
      break;
    case 2:
      a=intersection[0],b=intersection[1];
      norm[2] = normVal;
      norm.normalize();
      break;
  }


  // Determine whether the intersection lies within the plane
  if (a <= aMax && a >= aMin && b <= bMax && b >= bMin) {
    in = norm;
    return true;
  }

  return false;
}

bool setRayIntersction(Ray3D& ray, const Matrix4x4& worldToModel,
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

bool setRayIntersction(Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld, double t, Vector3D norm,
    Point3D intersection, double u, double v) {
  if(ray.intersection.none || t < ray.intersection.t_value) {
    ray.intersection.t_value = t;
    ray.intersection.point = modelToWorld * intersection;
    ray.intersection.normal = transNorm(worldToModel, norm);
    ray.intersection.normal.normalize();
    ray.intersection.none = false;

    ray.intersection.u = u;
    ray.intersection.v = v;

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

	// Determine whether the intersection lies within the square
  double t;
  Vector3D norm;
  Point3D intersection;
	if (intersectsPlane(mRay, t, norm, intersection, 2, 1, 0.0, -0.5, 0.5, -0.5, 0.5)) {
    double u = 0.5 + intersection[0];
    double v = 0.5 + intersection[1];

		return setRayIntersction(ray, worldToModel, modelToWorld, t, norm, intersection, u, v);
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

  double u = 0.5 + atan2(intersection[1], intersection[0]) / (2.0 * M_PI);
  double v = 0.5 - asin(intersection[2]) / M_PI;

	// Determine whether the intersection lies within the square
	// Relax the t_value
	Vector3D norm = intersection - center;

	// update
  return setRayIntersction(ray, worldToModel, modelToWorld, t, norm, intersection, u, v);
}

// the circle is in the XY plane, and has radius 1
bool intersectsXYCircle(Ray3D& ray, double circleZ, double *t, Point3D *intersection) {
  *t = (circleZ - ray.origin[2]) / (ray.dir[2]);

  // if t is behind the view plane, return false
  if(*t <= 0)
    return false;

  // Find x,y co-ordinates of the intersection point
  *intersection = ray.origin + (*t) * ray.dir;

  Vector3D xy ((*intersection)[0], (*intersection)[1], 0);
  double intersectionRadius = xy.squaredLength();

  // we do not need to check t, since we've designed t to set the intersection's
  // z coordinate equal to the circle's z coordinate
  // however we need to check that the intersection point is within the circle
  return intersectionRadius < 1;
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
  if(solutionFound && t > 0) {
    // Find x,y co-ordinates of the intersection point
    Point3D intersection = mRay.origin + t * mRay.dir;

    // Determine whether the intersection lies within the square
    if (intersection[2] < 1 && intersection[2] > -1) {
      Vector3D norm(intersection[0], intersection[1], 0);

      return setRayIntersction(ray, worldToModel, modelToWorld, t, norm, intersection);
    }
  }

  // (2) and (3) top and bottom of cylinder
  double t_top, t_bottom;
  Point3D i_top, i_bottom;
  bool intersectsTop = intersectsXYCircle(mRay, 1, &t_top, &i_top);
  bool intersectsBottom = intersectsXYCircle(mRay, -1, &t_bottom, &i_bottom);

  bool updated = false;

  if(intersectsTop && t_top < t_bottom) {
    Vector3D norm(0, 0, 1);
    updated = setRayIntersction(ray, worldToModel, modelToWorld, t_top, norm, i_top);
  }
  if(intersectsBottom && t_bottom < t_top) {
    Vector3D norm(0, 0, -1);
    updated = setRayIntersction(ray, worldToModel, modelToWorld, t_bottom, norm, i_bottom);
  }

  return updated;
}

enum ParseMode { VERTEX, NORMAL, FACE };

// parse an OBJ file
Mesh::Mesh(const char *filename) : name(filename) {
  ParseMode currentMode = VERTEX;
  std::ifstream file;
  file.open(filename, std::ios::in);

  if(file.is_open()) {
    std::string line;
    // parse the files
    while (std::getline(file, line)) {
      // skip comments and blank lines
      if(line[0] == '#' || line[0] == '\n')
        continue;

      // parse faces
      if(line[0] == 'f')
        currentMode = FACE;
      // ignore texture coordinates
      else if(line[0] == 'v' && line[1] == 't')
        continue;
      // ignore normals
      else if(line[0] == 'v' && line[1] == 'n')
        currentMode = NORMAL;
      // just in case the order of things may change
      else if(line[0] == 'v')
        currentMode = VERTEX;

      switch(currentMode) {
        case VERTEX:
          parseVertex(line);
          break;
        case NORMAL:
          parseNormal(line);
          break;
        case FACE:
          parseFace(line);
          break;
      }
    }
  }

  std::cout << filename << std::endl;
  std::cout << "x min: " << xMin << " max: " << xMax << std::endl;
  std::cout << "y min: " << yMin << " max: " << yMax << std::endl;
  std::cout << "z min: " << zMin << " max: " << zMax << std::endl;
  std::cout << "vertices: " << vertices.size() << std::endl;
  std::cout << "faces: " << faces.size() << std::endl;

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

  // update min and max
  xMin = std::min(xMin, x);
  xMax = std::max(xMax, x);

  yMin = std::min(yMin, y);
  yMax = std::max(yMax, y);

  zMin = std::min(zMin, z);
  zMax = std::max(zMax, z);

  vertices.push_back(parsed);

  return true;
}

bool Mesh::parseNormal(std::string& line) {
  // skip the first character, since we already know what it is
  std::istringstream iss(line);
  iss.seekg(2);

  double x,y,z;
  // must have an x,y,z
  if(!(iss >> x >> y >> z)) {
    return false;
  }

  // try to parse optional w
  Vector3D parsed(x,y,z);

  normals.push_back(parsed);

  return true;
}

bool Mesh::parseFace(std::string& line) {
  if(line[0] != 'f')
    return false;

  // skip the first character
  std::istringstream iss(line);
  iss.seekg(1);

  std::vector< std::pair<int, int> > face;
  int vertexIndex, normalIndex, empty;

  while(iss >> vertexIndex) {
    // 47 is forward slash
    // ignore texture coordinate
    if(iss.peek() == 47) {
      iss.ignore(); // skip the slash

      // the number for texture coordinate is optional
      if(iss.peek() != 47) {
        iss >> empty;
      }
    }

    // ignore normal coordinate
    if(iss.peek() == 47) {
      iss.ignore(); // skip the slash
      iss >> normalIndex;
    }

    face.push_back(std::make_pair(vertexIndex-1, normalIndex-1));
  }

  faces.push_back(face);

  return true;
}

// bool roughIntersection(Ray3D &oRay, Ray3D &ray, const Matrix4x4& worldToModel,
//     const Matrix4x4& modelToWorld) {
bool roughIntersection(Ray3D &ray) {

  // needed for intersectsPlane
  double ts[6] = { 0, 0, 0, 0, 0, 0 };
  Vector3D ns[6];
  Point3D is[6];
  bool intersects = false;

  intersects =
  // xs
      intersectsPlane(ray, ts[0], ns[0], is[0], 0, -1, 0, -0.5, 0.5, -0.5, 0.5)
  ||  intersectsPlane(ray, ts[1], ns[1], is[1], 0,  1, 1, -0.5, 0.5, -0.5, 0.5)

  // ys
  ||  intersectsPlane(ray, ts[2], ns[2], is[2], 1, -1, 0, -0.5, 0.5, -0.5, 0.5)
  ||  intersectsPlane(ray, ts[3], ns[3], is[3], 1,  1, 1, -0.5, 0.5, -0.5, 0.5)

  // zs
  ||  intersectsPlane(ray, ts[4], ns[4], is[4], 2, -1, 0, -0.5, 0.5, -0.5, 0.5)
  ||  intersectsPlane(ray, ts[5], ns[5], is[5], 2,  1, 1, -0.5, 0.5, -0.5, 0.5);

  // get the smallest t, that is greater > 0 and less than (or equal to) the max
  int idx = 0;
  double t = *std::max_element(ts, ts+6);
  for(int i = 0; i < 6; i++) {
    if(ts[i] > 0 && ts[i] <= t) {
      idx = i;
    }
    // std::cout << ts[i] << " ";
  }
  // std::cout << std::endl;
  // std::cout << idx << std::endl;

  return intersects && ts[idx] > 0;
}

bool Mesh::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld ) {

  Ray3D mRay;
  mRay.origin = worldToModel * ray.origin;
  mRay.dir = worldToModel * ray.dir;

  // Meshes must be between [-0.5,0.5] for all vertices.
  // test this by normalizing the size of your mesh in a model editor
  // and then view the output of the mesh constructor. it should
  // print -0.5, 0.5 for min / max for all axes for this to work efficiently.
  // return roughIntersection(ray, mRay, worldToModel, modelToWorld);
  if(!roughIntersection(mRay)) {
    return false;
  }

  bool updated = false;

  // calculate the normal for each face,
  // assume each face is a triangle only for simplicity
  int j = 0;
  for( auto &face : faces ) {
    j++;

    // calculate the face normal from each normal in the face
    Vector3D n;
    for( auto &pair : face ) {
      n = n + normals[pair.second];
    }
    // Point3D a = vertices[face[0].first];
    // Point3D b = vertices[face[1].first];
    // Point3D c = vertices[face[2].first];

    // Vector3D n = (b - a).cross(c - a);
    n.normalize();

    // calculate line sphere intersection
    // get a point on the face's plane
    Point3D p_0 = vertices[face[0].first];
    Point3D l_0 = mRay.origin;
    Vector3D l = mRay.dir;

    // calculate the "t" for the line, using
    // http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection

    // (p_0 - l_0) dot n
    double num = (p_0 - l_0).dot(n);

    // l dot n
    double den = (l.dot(n));

    // case (1) (i) line is not contained in plane, and is parallel
    // case (1) (ii) line is contained in plane, and is parallel
    if(den == 0) {
      continue;
    }

    double t = num / den;

    // if there has been a previous intersection that is closer
    // if t <= 0, then we are behind the ray's start point, which is behind the view
    // plane
    if(t <= 0 || (!ray.intersection.none && t > ray.intersection.t_value)) {
      continue;
    }

    // calculate the intersection point
    Point3D i = l_0 + t * l;

    // check that the intersection point is on the plane, inside the face
    // using the algorithm on A1
    int len = face.size();

    // check if it is inside the face
    bool insideFace = true;
    for( int k = 0; k < len && insideFace; k++) {

      // using the algorithm from A1
      Point3D v_1 = vertices[face[k].first];
      Point3D v_2 = vertices[face[(k+1) % len].first];

      Vector3D v = v_2 - v_1;
      v.normalize();

      // normal cross v, ensures we get a vector that is perpendicular
      // to the line segment from v_1 to v_2, and is inside the plane,
      // and is also pointing towards the inside of the face
      insideFace = n.cross(v).dot(i - v_1) >= 0;
    }

    // finally set the intersection point
    if(insideFace) {
      updated = setRayIntersction(ray, worldToModel, modelToWorld, t, n, i)
        || updated;
    }
  }

  return updated;
}
