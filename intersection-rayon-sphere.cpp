#include <iostream>
#include "Sphere.h"
#include "Ray.h"
#include "lib/lodepng.h"

float hit_sphere(Sphere sphere, Ray ray) {
	Vector3 oc = ray.origin - sphere.position;
	float a = Vector3::dot(ray.direction, ray.direction);
	float b = 2.0 * Vector3::dot(oc, ray.direction);
	float c = Vector3::dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1; // Use proper way to return bad value ( std::optional )
	else 
		return(-b - sqrt(discriminant)) / (2.0 * a);
}

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void setColor(std::vector<unsigned char> &image, int index, int r, int g, int b, int alpha = 255) {
	image[index] = r;			// Red
	image[index + 1] = g;		// Green
	image[index + 2] = b;		// Blue
	image[index + 3] = alpha;	// Alpha
}

int main() {

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vector3(200, 200, 50), 25));
	spheres.push_back(Sphere(Vector3(140, 160, 50), 15));
	Vector3 lightPosition = Vector3(10, 25, 50);

	unsigned width = 256, height = 256;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);


	for (unsigned x = 0; x < width; x++) {
		for (unsigned y = 0; y < height; y++) {
			int index = 4 * width * y + 4 * x;
			Ray r = Ray(Vector3(x, y, 0), Vector3(0, 0, 1));
			setColor(image, 4 * width * y + 4 * x, 255, 255, 255);

			for (Sphere& aSphere : spheres) {
				float dist = hit_sphere(aSphere, r);
				if (dist >= 0) {
					setColor(image, index, 127, 127, 127);

					Vector3 p = Vector3(x, y, dist);

					Ray _r = Ray(p, (lightPosition - p).normalized());
					for (Sphere& _aSphere : spheres) {
						float dist_otherSphere = hit_sphere(_aSphere, _r);
						if (dist_otherSphere >= 0) // If the ray hits another sphere
							setColor(image, index, 0, 0, 0);
					}
				}
			}
		}
	}

	encodeOneStep("image.png", image, width, height);
}