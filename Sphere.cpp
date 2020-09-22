#include "sphere.h"

Sphere::Sphere(Vector3 p, float r, bool isMirror, Color c) {
	position = p;
	radius = r;
	color = c;
	this->isMirror = isMirror;
}