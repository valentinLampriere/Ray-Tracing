#include "lib/vector.h"

#ifndef SPHERE_H
#define SPHERE_H

using namespace std;

class Sphere {
public:
	Vector3 position;
	float radius;

	Sphere(Vector3 o, float r);
};

#endif