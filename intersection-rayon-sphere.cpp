#include <iostream>
#include "lib/vector.cpp"
#include "lib/lodepng.cpp"

float hit_sphere(Vector3 centerSphere, float radiusSphere, Vector3 startRay, Vector3 directionRay) {
	Vector3 oc = startRay - centerSphere;
	float a = Vector3::dot(directionRay, directionRay);
	float b = 2.0 * Vector3::dot(oc, directionRay);
	float c = Vector3::dot(oc, oc) - radiusSphere * radiusSphere;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1; // Use proper way to return bad value ( std::optional )
	else 
		return(-b - std::sqrt(discriminant)) / (2.0 * a);
}

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

int main() {

	Vector3 sphereCenter = Vector3(125, 125, 100);
	float	sphereRadius = 50;

	unsigned width = 256, height = 256;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (unsigned x = 0; x < width; x++) {
		for (unsigned y = 0; y < height; y++) {
			int index = 4 * width * y + 4 * x;
			
			float c = (hit_sphere(sphereCenter, sphereRadius, Vector3(x, y, 0), Vector3(0, 0, 1)) >= 0) ? 0 : 255;

			image[index] = c;		// Red
			image[index + 1] = c;	// Green
			image[index + 2] = c;	// Blue
			image[index + 3] = 255; // Alpha

		}
	}

	encodeOneStep("image.png", image, width, height);
}