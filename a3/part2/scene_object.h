/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		classes defining primitives in the scene

***********************************************************/

#include "util.h"
#include <vector>

// All primitives should provide a intersection function.
// To create more primitives, inherit from SceneObject.
// Namely, you can create, Sphere, Cylinder, etc... classes
// here.
class SceneObject {
public:
	// Returns true if an intersection occured, false otherwise.
	virtual bool intersect( Ray3D&, const Matrix4x4&, const Matrix4x4& ) = 0;
};

// Example primitive you can create, this is a unit square on
// the xy-plane.
class UnitSquare : public SceneObject {
public:
	bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
			const Matrix4x4& modelToWorld );
};

class UnitSphere : public SceneObject {
public:
	bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
			const Matrix4x4& modelToWorld );
};

class UnitCylinder : public SceneObject {
public:
  bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
      const Matrix4x4& modelToWorld );
};

class Mesh : public SceneObject {
public:
  Mesh(const char *filename);

  bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
      const Matrix4x4& modelToWorld );

  const std::string name;

private:
  bool parseVertex(std::string&);
  bool parseNormal(std::string&);
  bool parseFace(std::string&);

  // TODO remove these
  double xMin = -0.5;
  double xMax = 0.5;

  double yMin = -0.5;
  double yMax = 0.5;

  double zMin = -0.5;
  double zMax = 0.5;

  std::vector<Point3D> vertices;
  std::vector<Vector3D> normals;
  // pair of vertices,normals per face
  std::vector< std::vector< std::pair<int, int> > > faces;
};
