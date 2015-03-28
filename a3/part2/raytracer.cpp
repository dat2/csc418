/***********************************************************
		 Starter code for Assignment 3

		 This code was originally written by Jack Wang for
				CSC418, SPRING 2005

		Implementations of functions in raytracer.h,
		and the main function which specifies the
		scene to be rendered.

***********************************************************/


#include "raytracer.h"
#include "bmp_io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <time.h>

Raytracer::Raytracer() : _lightSource(NULL) {
	_root = new SceneDagNode();
}

Raytracer::~Raytracer() {
	delete _root;
}

SceneDagNode* Raytracer::addObject( SceneDagNode* parent,
		SceneObject* obj, Material* mat ) {
	SceneDagNode* node = new SceneDagNode( obj, mat );
	node->parent = parent;
	node->next = NULL;
	node->child = NULL;

	// Add the object to the parent's child list, this means
	// whatever transformation applied to the parent will also
	// be applied to the child.
	if (parent->child == NULL) {
		parent->child = node;
	}
	else {
		parent = parent->child;
		while (parent->next != NULL) {
			parent = parent->next;
		}
		parent->next = node;
	}

	return node;;
}

LightListNode* Raytracer::addLightSource( LightSource* light ) {
	LightListNode* tmp = _lightSource;
	_lightSource = new LightListNode( light, tmp );
	return _lightSource;
}

void Raytracer::rotate( SceneDagNode* node, char axis, double angle ) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;

	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
				node->trans = node->trans*rotation;
			angle = -angle;
		}
		else {
			node->invtrans = rotation*node->invtrans;
		}
	}
}

void Raytracer::translate( SceneDagNode* node, Vector3D trans ) {
	Matrix4x4 translation;

	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	node->trans = node->trans*translation;
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	node->invtrans = translation*node->invtrans;
}

void Raytracer::scale( SceneDagNode* node, Point3D origin, double factor[3] ) {
	Matrix4x4 scale;

	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	node->trans = node->trans*scale;
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	node->invtrans = scale*node->invtrans;
}

Matrix4x4 Raytracer::initInvViewMatrix( Point3D eye, Vector3D view,
		Vector3D up ) {
	Matrix4x4 mat;
	Vector3D w;
	view.normalize();
	up = up - up.dot(view)*view;
	up.normalize();
	w = view.cross(up);

	mat[0][0] = w[0];
	mat[1][0] = w[1];
	mat[2][0] = w[2];
	mat[0][1] = up[0];
	mat[1][1] = up[1];
	mat[2][1] = up[2];
	mat[0][2] = -view[0];
	mat[1][2] = -view[1];
	mat[2][2] = -view[2];
	mat[0][3] = eye[0];
	mat[1][3] = eye[1];
	mat[2][3] = eye[2];

	return mat;
}

void Raytracer::traverseScene( SceneDagNode* node, Ray3D& ray ) {
	SceneDagNode *childPtr;

	// Applies transformation of the current node to the global
	// transformation matrices.
	_modelToWorld = _modelToWorld*node->trans;
	_worldToModel = node->invtrans*_worldToModel;
	if (node->obj) {
		// Perform intersection.
		if (node->obj->intersect(ray, _worldToModel, _modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
	// Traverse the children.
	childPtr = node->child;
	while (childPtr != NULL) {
		traverseScene(childPtr, ray);
		childPtr = childPtr->next;
	}

	// Removes transformation of the current node from the global
	// transformation matrices.
	_worldToModel = node->trans*_worldToModel;
	_modelToWorld = _modelToWorld*node->invtrans;
}

void Raytracer::computeShading( Ray3D& ray ) {
	LightListNode* curLight = _lightSource;
	for (;;) {
		if (curLight == NULL) break;
		// Each lightSource provides its own shading function.

		// Implement shadows here if needed.
		Ray3D sRay;
		sRay.dir = curLight->light->get_position() - ray.intersection.point;
		sRay.origin = ray.intersection.point + 0.001 * sRay.dir;

		traverseScene(_root, sRay);
		bool isShadow = !sRay.intersection.none 
			&& sRay.dir.length() >= sRay.intersection.t_value;

		curLight->light->shade(ray, isShadow);
		curLight = curLight->next;
	}
}

void Raytracer::initPixelBuffer() {
	int numbytes = _scrWidth * _scrHeight * sizeof(unsigned char);
	_rbuffer = new unsigned char[numbytes];
	_gbuffer = new unsigned char[numbytes];
	_bbuffer = new unsigned char[numbytes];
	for (int i = 0; i < _scrHeight; i++) {
		for (int j = 0; j < _scrWidth; j++) {
			_rbuffer[i*_scrWidth+j] = 0;
			_gbuffer[i*_scrWidth+j] = 0;
			_bbuffer[i*_scrWidth+j] = 0;
		}
	}
}

void Raytracer::flushPixelBuffer( char *file_name ) {
	bmp_write( file_name, _scrWidth, _scrHeight, _rbuffer, _gbuffer, _bbuffer );
	delete _rbuffer;
	delete _gbuffer;
	delete _bbuffer;
}

Colour Raytracer::shadeRay( Ray3D& ray, int trace_level ) {
	Colour col(0.0, 0.0, 0.0);
	Colour reflCol(0.0, 0.0, 0.0);
	Colour refrCol(0.0, 0.0, 0.0); 
	traverseScene(_root, ray);

	// Don't bother shading if the ray didn't hit
	// anything.
	if (ray.intersection.none)
		return col;

	// TODO: You'll want to call shadeRay recursively (with a different ray,
	// of course) here to implement reflection/refraction effects.
	computeShading(ray);
	if (trace_level != 0) {

		Intersection &inter = ray.intersection;
        Vector3D s = inter.point - ray.origin;
        Vector3D n = inter.normal;
        s.normalize();

        // Reflection
        if (inter.mat->specular_exp > 0 && inter.mat->reflection_coeff > 0){
        	double epsilon(0.001);
            Ray3D reflRay;
            reflRay.dir = s - 2 * ((s.dot(n)) * n);
            reflRay.dir.normalize();
            reflRay.origin = inter.point + epsilon * reflRay.dir;
            reflCol = shadeRay(reflRay, trace_level - 1);
            ray.col = ray.col + inter.mat->reflection_coeff * inter.mat->specular * reflCol;
        }

        // Refraction
        if ( inter.mat->refraction_coeff > 0){
        	double epsilon(0.0001);
        	double index = inter.mat->refractive_index;
        	if (trace_level % 2 == 0) // invert if outside sphere
            	 index = 1 / inter.mat->refractive_index;
            else  // take negative of normal if inside
            	n = -1 * n;
            double cos1 = -s.dot(n);
            double cos2 = 1 - pow(index, 2) * (1 - pow(cos1, 2));
            if (cos2 > 0.0) {
                Ray3D refrRay;
                refrRay.dir = index * s + (index * cos1 - sqrt(cos2)) * n;
                refrRay.origin = inter.point + epsilon * ray.dir;
                refrCol = shadeRay(refrRay, trace_level - 1);
            }
        	ray.col = (1 - inter.mat->refraction_coeff) * ray.col + inter.mat->refraction_coeff * refrCol;      
        }
	}

    col = ray.col;
    col.clamp();
    return col;
}

Colour Raytracer::getColourRay(Matrix4x4 viewToWorld, Point3D point) {
	// TODO: Convert ray to world space and call
	// shadeRay(ray) to generate pixel colour.
	Point3D origin(0, 0, 0);
	Ray3D ray;

	// Move view origin to world space
	ray.origin = viewToWorld * origin;

	// Calculate and set direction
	ray.dir = viewToWorld * point - ray.origin;
	ray.dir.normalize();

	return shadeRay(ray, RAYTRACE_LEVELS);
}

void Raytracer::render( int width, int height, Point3D eye, Vector3D view,
		Vector3D up, double fov, char* fileName ) {
	Matrix4x4 viewToWorld;
	_scrWidth = width;
	_scrHeight = height;
	double factor = (double(height)/2)/tan(fov*M_PI/360.0);

	initPixelBuffer();
	viewToWorld = initInvViewMatrix(eye, view, up);

	// Construct a ray for each pixel.
	for (int i = 0; i < _scrHeight; i++) {
		for (int j = 0; j < _scrWidth; j++) {
			// Sets up ray origin and direction in view space,
			// image plane is at z = -1.

			// Implementing Anti-aliasing
			Colour col;
			Point3D imagePlane;
			imagePlane[2] = -1;

			// shoot 9 rays
			#ifdef ANTIALIASING
			for (int u = -1; u < 2; u++){
				for (int v = -1; v < 2; v++){
					imagePlane[0] = (-double(width)/2 + 0.5 + u * 0.25 + j)/factor;
					imagePlane[1] = (-double(height)/2 + 0.5 + v * 0.25 + i)/factor;
					col = col + getColourRay(viewToWorld, imagePlane);
				}
			}

			col = (1./9.) * col;
			#else
				imagePlane[0] = (-double(width)/2 + 0.5 + j)/factor;
				imagePlane[1] = (-double(height)/2 + 0.5 + i)/factor;
				col =  getColourRay(viewToWorld, imagePlane);
			#endif

			_rbuffer[i*width+j] = int(col[0]*255);
			_gbuffer[i*width+j] = int(col[1]*255);
			_bbuffer[i*width+j] = int(col[2]*255);
		}
	}

	flushPixelBuffer(fileName);
}

time_t t_i, t_a;

void printBeforeTime() {
  time(&t_i);
  struct tm *timeinfo;
  timeinfo = localtime(&t_i);

  std::cout << "Time before rendering: " << asctime(timeinfo);
}

void printAfterTime() {
  time(&t_a);
  struct tm *timeinfo;
  timeinfo = localtime(&t_a);

  std::cout << "Time after rendering: " << asctime(timeinfo);
  std::cout << "Total time elapsed: " << difftime(t_a,t_i) << "s" << std::endl;
}

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the
	// assignment.
	Raytracer raytracer;
	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

  /** Materials */
	Material light_gold_shiny( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648),
			Colour(0.128281, 0.155802, 0.166065),
			12.8, 0.8);

	Material light_gold( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648),
			Colour(0.628281, 0.555802, 0.366065),
			51.2);

Material emerald( Colour(0.0215, 0.1745, 0.0215), Colour(0.07568, 0.61424, 0.07568),
			 Colour(0.633, 0.727811, 0.633),
			76.80);
Material jade( Colour(0.135, 0.2225, 0.1575), Colour(0.54, 0.89, 0.63),
			 Colour(0.316228, 0.316228, 0.316228),
			12.80);
Material obsidian( Colour(0.05375, 0.05, 0.06625), Colour(0.18275, 0.17, 0.22525),
			 Colour(0.332741, 0.328634, 0.346435),
			38.40);
Material pearl( Colour(0.25, 0.20725, 0.20725), Colour(1, 0.829, 0.829),
			 Colour(0.296648, 0.296648, 0.296648),
			11.26);
Material ruby( Colour(0.1745, 0.01175, 0.01175), Colour(0.61424, 0.04136, 0.04136),
			 Colour(0.727811, 0.626959, 0.626959),
			76.80);
Material turquoise( Colour(0.1, 0.18725, 0.1745), Colour(0.396, 0.74151, 0.69102),
			 Colour(0.297254, 0.30829, 0.306678),
			12.80);
Material brass( Colour(0.329412, 0.223529, 0.027451), Colour(0.780392, 0.568627, 0.113725),
			 Colour(0.992157, 0.941176, 0.807843),
			27.90);
Material bronze( Colour(0.2125, 0.1275, 0.054), Colour(0.714, 0.4284, 0.18144),
			 Colour(0.393548, 0.271906, 0.166721),
			25.60);
Material chrome( Colour(0.25, 0.25, 0.25), Colour(0.4, 0.4, 0.4),
			 Colour(0.774597, 0.774597, 0.774597),
			76.80);
Material copper( Colour(0.19125, 0.0735, 0.0225), Colour(0.7038, 0.27048, 0.0828),
			 Colour(0.256777, 0.137622, 0.086014),
			12.80);
Material gold( Colour(0.24725, 0.1995, 0.0745), Colour(0.75164, 0.60648, 0.22648),
			 Colour(0.628281, 0.555802, 0.366065),
			51.20);
Material silver( Colour(0.19225, 0.19225, 0.19225), Colour(0.50754, 0.50754, 0.50754),
			 Colour(0.508273, 0.508273, 0.508273),
			51.20);
Material black_plastic( Colour(0.0, 0.0, 0.0), Colour(0.01, 0.01, 0.01),
			 Colour(0.50, 0.50, 0.50),
			32.00);
Material cyan_plastic( Colour(0.0, 0.1, 0.06), Colour(0.0, 0.50980392, 0.50980392),
			 Colour(0.50196078, 0.50196078, 0.50196078),
			32.00);
Material green_plastic( Colour(0.0, 0.0, 0.0), Colour(0.1, 0.35, 0.1),
			 Colour(0.45, 0.55, 0.45),
			32.00);
Material red_plastic( Colour(0.0, 0.0, 0.0), Colour(0.5, 0.0, 0.0),
			 Colour(0.7, 0.6, 0.6),
			32.00);
Material white_plastic( Colour(0.0, 0.0, 0.0), Colour(0.55, 0.55, 0.55),
			 Colour(0.70, 0.70, 0.70),
			32.00);
Material yellow_plastic( Colour(0.0, 0.0, 0.0), Colour(0.5, 0.5, 0.0),
			 Colour(0.60, 0.60, 0.50),
			32.00);
Material black_rubber( Colour(0.02, 0.02, 0.02), Colour(0.01, 0.01, 0.01),
			 Colour(0.4, 0.4, 0.4),
			10.00);
Material cyan_rubber( Colour(0.0, 0.05, 0.05), Colour(0.4, 0.5, 0.5),
			 Colour(0.04, 0.7, 0.7),
			10.00);
Material green_rubber( Colour(0.0, 0.05, 0.0), Colour(0.4, 0.5, 0.4),
			 Colour(0.04, 0.7, 0.04),
			10.00);
Material red_rubber( Colour(0.05, 0.0, 0.0), Colour(0.5, 0.4, 0.4),
			 Colour(0.7, 0.04, 0.04),
			10.00);
Material white_rubber( Colour(0.05, 0.05, 0.05), Colour(0.5, 0.5, 0.5),
			 Colour(0.7, 0.7, 0.7),
			10.00);
Material yellow_rubber( Colour(0.05, 0.05, 0.0), Colour(0.5, 0.5, 0.4),
			 Colour(0.7, 0.7, 0.04),
			10.00);

	Material glass( Colour(0.0, 0.0, 0.0), Colour(0.4, 0.4, 0.4),
			Colour(0.8, 0.6, 0.8),
			52.8, 0.1, 1.0, 1.42 );

	Material gold_glass( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648),
			Colour(0.628281, 0.555802, 0.366065),
			51.2, 0.5, 0.5, 1.4);

	// Origin
	Point3D origin(0, 0, 0);

  // moon material is silver
  Material moonMaterial( Colour(0.19225, 0.19225, 0.19225), Colour(0.50754, 0.50754, 0.50754),
       Colour(0.508273, 0.508273, 0.508273),
      51.20);
  Texture texture("textures/moonmap.bmp");
  moonMaterial.texture = &texture;
  moonMaterial.hasTexture = true;

  /** SPHERES */
 //  SceneDagNode* goldEgg = raytracer.addObject( new UnitSphere(), &gold );
	// double factor1[3] = { 1.0, 2.0, 1.0 };
	// raytracer.translate(goldEgg, Vector3D(3, 0, 3));
	// raytracer.rotate(goldEgg, 'x', 45);
	// raytracer.rotate(goldEgg, 'z', 45);
	// raytracer.scale(goldEgg, origin, factor1);

 //  SceneDagNode* redGlassSphere = raytracer.addObject( new UnitSphere(), &gold_glass );
	// double factor_circle[3] = { 0.75, 0.75, 0.75 };
	// raytracer.translate(redGlassSphere, Vector3D(8, -7, 5));
	// raytracer.scale(redGlassSphere, origin, factor_circle);

  SceneDagNode* moon = raytracer.addObject( new UnitSphere(), &moonMaterial );
	// double factor_circle_big[3] = { 2, 2, 2 };
	raytracer.translate(moon, Vector3D(-5, -5, 3));
	// raytracer.scale(moon, origin, factor_circle_big);

  /** WALLS */
  double plane_scale[3] = { 20.0, 20.0, 20.0 };
  SceneDagNode* floor = raytracer.addObject( new UnitSquare(), &pearl );
	raytracer.scale(floor, origin, plane_scale);

 //  SceneDagNode* leftWall = raytracer.addObject( new UnitSquare(), &pearl );
	// raytracer.translate(leftWall, Vector3D(-10, 0, 10));
	// raytracer.rotate(leftWall, 'y', 90);
	// raytracer.scale(leftWall, origin, plane_scale);

 //  SceneDagNode* rightWall = raytracer.addObject( new UnitSquare(), &pearl );
	// raytracer.translate(rightWall, Vector3D(0, 10, 10));
	// raytracer.rotate(rightWall, 'x', 90);
	// raytracer.scale(rightWall, origin, plane_scale);

	/** TABLE **/
  SceneDagNode* table = raytracer.addObject(NULL, NULL);
  SceneDagNode* tableTop = raytracer.addObject( table, new UnitCylinder(), &obsidian );
  SceneDagNode* tableLeg1 = raytracer.addObject( table, new UnitCylinder(), &obsidian );
  SceneDagNode* tableLeg2 = raytracer.addObject( table, new UnitCylinder(), &obsidian );
  SceneDagNode* tableLeg3 = raytracer.addObject( table, new UnitCylinder(), &obsidian );
  SceneDagNode* tableLeg4 = raytracer.addObject( table, new UnitCylinder(), &obsidian );

	raytracer.translate(table, Vector3D(0, 0, 2));

	double tableTop_scale[3] = { 3.0, 3.0, 0.1 };
	raytracer.scale(tableTop, origin, tableTop_scale);

	double tableLeg_scale[3] = { 0.2, 0.2, 1.0 };

	raytracer.translate(tableLeg1, Vector3D(1.8, -1.8, -1));
	raytracer.scale(tableLeg1, origin, tableLeg_scale);

	raytracer.translate(tableLeg2, Vector3D(-1.8, -1.8, -1));
	raytracer.scale(tableLeg2, origin, tableLeg_scale);

	raytracer.translate(tableLeg3, Vector3D(1.8, 1.8, -1));
	raytracer.scale(tableLeg3, origin, tableLeg_scale);

	raytracer.translate(tableLeg4, Vector3D(-1.8, 1.8, -1));
	raytracer.scale(tableLeg4, origin, tableLeg_scale);

	/** BUNNY */
  SceneDagNode* bunny = raytracer.addObject( new Mesh("meshes/bunny.obj"), &gold );

	double bunnyFactor[3] = { 3.0, 3.0, 3.0 };
	raytracer.translate(bunny, Vector3D(4,0,7));
	raytracer.rotate(bunny, 'z', 90);
	// raytracer.rotate(bunny, 'x', 90);
	raytracer.scale(bunny, Point3D(0, 0, 0), bunnyFactor);

  /** TEAPOT */
  SceneDagNode* teapot = raytracer.addObject( new Mesh("meshes/teapot.obj"), &chrome );

  double teapotFactor[3] = { 3.0, 3.0, 3.0 };
  raytracer.translate(teapot, Vector3D(0,0,3));
  raytracer.scale(teapot, Point3D(0, 0, 0), teapotFactor);

 //  /** MICKEY */
  double mickeyScale[] = { 3.0, 3.0, 3.0 };
  SceneDagNode* mickey = raytracer.addObject( new Mesh("meshes/mickey.obj"), &ruby);
	raytracer.translate(mickey, Vector3D(6, 0, 3));
  raytracer.scale(mickey, origin, mickeyScale);

	// Render the scene, feel free to make the image smaller for
	// testing purposes.

	// Defines a point light source.
	raytracer.addLightSource( new PointLight(Point3D(0, 0, 10),
				Colour(0.9, 0.9, 0.9) ) );

	// Camera parameters.
	Point3D eye(10, -10, 8);
	Vector3D view(-1, 1, -1);
	Vector3D up(0, 0, 1);
	double fov = 60;

  std::cout << std::endl;
 //  printBeforeTime();
	// raytracer.render(width, height, eye, view, up, fov, "views/view1.bmp");
 //  printAfterTime();
 //  std::cout << std::endl;

	// Render it from a different point of view.
	Point3D eye2(0, -15, 3);
	Vector3D view2(0, 2, 0);

  printBeforeTime();
	raytracer.render(width, height, eye2, view2, up, fov, "views/view2.bmp");
  printAfterTime();

	return 0;
}

