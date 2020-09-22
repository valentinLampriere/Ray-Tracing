#include <iostream>
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "lib/lodepng.h"

#define _USE_MATH_DEFINES
#include <math.h>

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

void setColor(std::vector<unsigned char>& image, int index, Color c) {
	image[index] = c.r * 255;			// Red
	image[index + 1] = c.g * 255;		// Green
	image[index + 2] = c.b * 255;		// Blue
	image[index + 3] = 255;		// Blue
}

std::vector<Sphere> spheres;
std::vector<Light> lightsSources;

std::vector<unsigned char> image;

Color calcLuminosityAtPoint(Vector3 point, Sphere s, Light l) {
	float V = 1;
	Vector3 N = (point - s.position).normalized(); // Normale on "point"

	Vector3 dir = (l.position - point).normalized();

	Ray _r = Ray(point + dir * +0.01f, dir);

	float distance2 = (l.position - point).lengthSquare();

	for (Sphere& _aSphere : spheres) {
		float dist_otherSphere = hit_sphere(_aSphere, _r);
		if (dist_otherSphere >= 0 && dist_otherSphere * dist_otherSphere < distance2) { // If the ray hits another sphere
			return Color(0, 0, 0);
		}
	}

	return (l.GetColor() * s.color * std::abs(N.dot(dir))) / (distance2 * M_PI);
}

int main() {

	// ADD SPHERES
	spheres.push_back(Sphere(Vector3(200, 200, 200), 60));
	spheres.push_back(Sphere(Vector3(90, 130, 200), 25));
	spheres.push_back(Sphere(Vector3(50, 65, 200), 10));

	// ADD LIGHTS
	lightsSources.push_back(Light(Vector3(0, 0, 200), Color(1, 0, 0), 500000));
	//lightsSources.push_back(Light(Vector3(300, 0, 200), Color(0, 250000, 500000)));


	unsigned width = 256, height = 256;
	image.resize(width * height * 4);

	for (unsigned x = 0; x < width; x++) {
		for (unsigned y = 0; y < height; y++) {
			int index = 4 * width * y + 4 * x;
			Ray r = Ray(Vector3(x, y, 0), Vector3(0, 0, 1));
			setColor(image, 4 * width * y + 4 * x, Color(0, 0.33, 0.8));

			for (Sphere& aSphere : spheres) {
				float dist = hit_sphere(aSphere, r);
				if (dist >= 0) {

					for (Light& aLight : lightsSources) {

						Color c = calcLuminosityAtPoint(Vector3(x, y, dist), aSphere, aLight);

						setColor(image, index, c);

						/*Vector3 p = Vector3(x, y, dist);
						Vector3 dir = (aLight.position - p).normalized();

						setColor(image, index, aLight.GetColor().Clamp());

						Ray _r = Ray(p + dir * -0.01f, dir);
						for (Sphere& _aSphere : spheres) {
							float dist_otherSphere = hit_sphere(_aSphere, _r);
							if (dist_otherSphere >= 0) // If the ray hits another sphere
								setColor(image, index,Color(0,0,0));
						}*/
					}
				}
			}
		}
	}

	encodeOneStep("image.png", image, width, height);
}