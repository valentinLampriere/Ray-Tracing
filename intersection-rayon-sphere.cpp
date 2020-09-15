#include <iostream>
#include "lib/vector.cpp"
#include "lib/lodepng.cpp"

void hit_sphere(Vector3 centerSphere, float radiusSphere, Vector3 startRay, Vector3 directionRay) {
	Vector3 oc = startRay - centerSphere;
	float a = Vector3::dot(directionRay, directionRay);
	float b = 2.0 * Vector3::dot(oc, directionRay);
	float c = Vector3::dot(oc, oc) - radiusSphere * radiusSphere;
	float discriminant = b * b - 4 * a * c;
}

int main() {
}