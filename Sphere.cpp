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
bool Sphere::operator==(const Sphere s) {
	return this->position.x == s.position.x && this->position.y == s.position.y && this->position.z == s.position.z && this->isMirror == s.isMirror && this->radius == s.radius;
}
bool Sphere::operator!=(const Sphere s) {
	return !(*this == s);
}