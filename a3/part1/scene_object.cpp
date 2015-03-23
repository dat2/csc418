/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

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
	if (x <= 0.5 && x >=-0.5 && y <= 0.5 && y >=-0.5){
		// Relax the t_value
		if (ray.intersection.none || t < ray.intersection.t_value){

			Vector3D norm(0, 0, 1);

			// update
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld * intersection;
			ray.intersection.normal = transNorm(worldToModel, norm);
			ray.intersection.normal.normalize();
			ray.intersection.none = false;

			return true;
		}
	}

	return false;
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

	// Calculate discriminant
	double discr = b * b - 4 * a * c;
	double t;

	// no intersection
	if (discr < 0)
		return false;
	else if (discr == 0)
		t = -b / (2 * a);
	else{
		double t1 = (-b + sqrt(discr) ) / (2 * a);
		double t2 = (-b - sqrt(discr) ) / (2 * a);
		t = fmin(t1, t2);
	}

	// return if t is in the reverse direction of the ray
	if (t <= 0)
		return false;

	// Find x,y co-ordinates of the intersection point
	Point3D intersection = mRay.origin + t * mRay.dir;

	// Determine whether the intersection lies within the square
	// Relax the t_value
	if (ray.intersection.none || t < ray.intersection.t_value){

		Vector3D norm = intersection - center;

		// update
		ray.intersection.t_value = t;
		ray.intersection.point = modelToWorld * intersection;
		ray.intersection.normal = transNorm(worldToModel, norm);
		ray.intersection.normal.normalize();
		ray.intersection.none = false;

		return true;
	}

	return false;
}

