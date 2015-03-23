/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

enum ShadingMode { SIG, DIFF, PHONG };
ShadingMode mode = PHONG;

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	Material *mat = ray.intersection.mat;
	Vector3D n = ray.intersection.normal;
	Vector3D s = _pos - ray.intersection.point; // light source direction
	Vector3D b = -ray.dir;	
	Vector3D r = 2 * n.dot(s) * n - s;

	// normalize
	s.normalize();
	b.normalize();
	r.normalize();

	Colour ambient = mat->ambient * _col_ambient;
	Colour diffuse = fmax(0, s.dot(n)) * mat->diffuse * _col_diffuse ;
	Colour specular = pow(fmax(0, r.dot(b)), mat->specular_exp) * mat->specular * _col_specular;

	// ray.col = ambient + diffuse + specular;
	switch(mode){
		case SIG:
			ray.col = ray.intersection.mat->diffuse;
			break;
		case DIFF:
			ray.col = ambient + diffuse;
			break;
		case PHONG:
			ray.col = ambient + diffuse + specular;
			break;
	}
	ray.col.clamp();
}

