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

	Sphere(Vector3 o, float r, Color c = Color(1,1,1));
};

#endif