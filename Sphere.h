#include "lib/vector.h"
#include "color.h"

#ifndef SPHERE_H
#define SPHERE_H

using namespace std;

class Sphere {
public:
	Vector3 position;
	float radius;
	Color color;
	bool isMirror;

	Sphere(Vector3 o, float r, Color c = Color(255, 255, 255));
	Sphere(Vector3 o, float r, bool isMirror);
	Vector3 normaleAtPoint(Vector3 point);
};

#endif