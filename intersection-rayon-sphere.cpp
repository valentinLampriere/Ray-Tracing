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

	Vector3 sphereCenter = Vector3(0, 0, 10);
	float	sphereRadius = 2;

	float planSize = 30;

	unsigned width = 256, height = 256;
	std::vector<unsigned char> image;
	image.resize(width * height);
	for (unsigned x = 0; x < width; x++) {
		for (unsigned y = 0; y < height; y++) {
			int index = x + y * width;
			image[index] = 255;
		}
	}

	lodepng::encode("image.png", image, width, height);
}