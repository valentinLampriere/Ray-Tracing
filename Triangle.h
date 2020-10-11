#pragma once

#include "lib/vector.h"
#include "Ray.h"

using namespace std;

class Triangle {
public:
	Vector3 point1, point2, point3;

	Triangle(Vector3 p1, Vector3 p2, Vector3 p3);
	bool rayHit(Ray r, float* distance);
};