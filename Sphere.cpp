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
void Sphere::getBoxCoord(Vector3* coord1, Vector3* coord2) {
	*coord1 = Vector3(position.x - radius, position.y - radius, position.z - radius);
	*coord2 = Vector3(position.x + radius, position.y + radius, position.z + radius);
}
bool Sphere::rayHit(Ray ray, float* d) {
	Vector3 oc = ray.origin - this->position;
	float a = Vector3::dot(ray.direction, ray.direction);
	float b = 2.0 * Vector3::dot(oc, ray.direction);
	float c = Vector3::dot(oc, oc) - this->radius * this->radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
		//return -1; // Use proper way to return bad value ( std::optional )
	} else {
		float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
		if (t1 > 0) {
			*d = t1;
			return true;
			//return t1;
		}
		if (t2 > 0) {
			*d = t2;
			return true;
			//return t2;
		}
		return false;
	}
}
bool Sphere::operator==(const Sphere s) {
	return this->position.x == s.position.x && this->position.y == s.position.y && this->position.z == s.position.z && this->isMirror == s.isMirror && this->radius == s.radius;
}
bool Sphere::operator!=(const Sphere s) {
	return !(*this == s);
}