/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray, bool isShadow ) {
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

	if (isShadow) {
    if(ray.intersection.mat->hasTexture) {
      double multiplier = ray.intersection.none ? 0.4 : 0.3;
      ray.col = multiplier * mat->texture->getColour(ray.intersection.u, ray.intersection.v) * _col_diffuse;
    } else {
      ray.col = 0.3 * mat->diffuse * _col_diffuse;
    }
  } else {
    Colour ambient = mat->ambient * _col_ambient;
    Colour diffuse;
    // Add gloss to specular
    Colour specular = pow(fmax(0, r.dot(b)), mat->specular_exp) * mat->specular * _col_specular;

    Colour materialDiffuse;
    if(ray.intersection.mat->hasTexture) {
      materialDiffuse = mat->texture->getColour(ray.intersection.u, ray.intersection.v);
    } else {
      materialDiffuse = mat->diffuse;
    }

    diffuse = fmax(0, s.dot(n)) * materialDiffuse * _col_diffuse;
    if(ray.intersection.mat->hasTexture) {
      ambient = diffuse * ambient;
    }

    ray.col = ambient + diffuse + specular;
	}
	ray.col.clamp();
}

