#pragma once

#include "lib/vector.h"
#include "Color.h"
#include "Ray.h"

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
	void getBoxCoord(Vector3* coord1, Vector3* coord2);
	bool rayHit(Ray r, float* d);
	bool operator==(const Sphere s);
	bool operator!=(const Sphere s);
};