#include "sphere.h"

Sphere::Sphere(Vector3 p, float r, Color c) {
	position = p;
	radius = r;
	color = c;
	isMirror = false;
}
Sphere::Sphere(Vector3 p, float r, bool isMirror) {
	position = p;
	radius = r;
	this->isMirror = isMirror;
}
Vector3 Sphere::normaleAtPoint(Vector3 point) {
	return (point - this->position).normalized();
}