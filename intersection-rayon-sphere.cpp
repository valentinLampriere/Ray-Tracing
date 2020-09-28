#include <iostream>
#include <optional>
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "lib/lodepng.h"

#define _USE_MATH_DEFINES
#include <math.h>

std::vector<Sphere> spheres;
std::vector<Light> lightsSources;

std::vector<unsigned char> image;

//unsigned width = 512, height = 512;


float hit_sphere(Sphere sphere, Ray ray) {
	Vector3 oc = ray.origin - sphere.position;
	float a = Vector3::dot(ray.direction, ray.direction);
	float b = 2.0 * Vector3::dot(oc, ray.direction);
	float c = Vector3::dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1; // Use proper way to return bad value ( std::optional )
	else {
		float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
		float t2 = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t1 > 0)
			return t1;
		if (t2 > 0)
			return t2;
		return -1;
	}
}

int hit_spheres(Ray ray, float* distance) {
	int closeSphereIndex = -1;
	for (int i = 0; i < spheres.size(); i ++) {
		float minDistance = hit_sphere(spheres[i], ray);
		if (minDistance != -1) {
			if (closeSphereIndex == -1) {
				closeSphereIndex = i;
				*distance = minDistance;
			} else {
				if (minDistance < *distance) {
					closeSphereIndex = i;
					*distance = minDistance;
				}
			}
		}
	}
	return closeSphereIndex;
}

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void setColor(std::vector<unsigned char>& image, int index, Color c) {
	image[index] = c.r;			// Red
	image[index + 1] = c.g;		// Green
	image[index + 2] = c.b;		// Blue
	image[index + 3] = 255;		// Alpha
}
Color calcLuminosityAtPoint(Vector3 point, Sphere s, Light l) {
	float V = 1;
	Vector3 N = (point - s.position).normalized(); // Normale on "point"

	Vector3 dir = (l.position - point).normalized();

	Ray _r = Ray(point + dir * 0.01f, dir);

	float distance2 = (l.position - point).lengthSquare();

	for (Sphere& _aSphere : spheres) {
		float dist_otherSphere = hit_sphere(_aSphere, _r);
		
		if (dist_otherSphere >= 0 && dist_otherSphere * dist_otherSphere < distance2) { // If the ray hits another sphere
			return Color(0,0,0);
		}
	}
	return (l.GetColor() * std::abs(N.dot(dir))) / (distance2 * M_PI);
}

Color reflectRay(Ray ray, Sphere sphere, Vector3 point) {
	Vector3 normale = sphere.normaleAtPoint(point);
	Vector3 directionNewRay = (normale * 2 * Vector3::dot(Vector3() - ray.direction, normale) + ray.direction).normalized();
	Ray newRay = Ray(point, directionNewRay);
	float distance;
	int indexSphere = hit_spheres(newRay, &distance);
	if (indexSphere == -1) {
		return Color(0, 0, 0);
	} else {
		if (spheres[indexSphere].isMirror) {
			reflectRay(newRay, spheres[indexSphere], point + directionNewRay * distance);
		}
		else {
			return Color(25, 225, 125);
		}
	}
}

Color manageLightReflection(Vector3 pointIntersection, Sphere s, int index) {
	Color colXY = Color(0, 0, 0);

	for (Light& aLight : lightsSources) {
		Color c = calcLuminosityAtPoint(pointIntersection, s, aLight);

		colXY = colXY + c;

		Vector3 dir = (aLight.position - pointIntersection).normalized();

		Ray _r = Ray(pointIntersection + dir * -0.01f, dir);
		for (Sphere& _aSphere : spheres) {
			if (_aSphere != s) {
				float dist_otherSphere = hit_sphere(_aSphere, _r);
				if (dist_otherSphere >= 0) { // If the ray hits another sphere
					//colXY = Color(0, 0, 0);
					setColor(image, index, Color(0, 0, 0));
				}
			}
		}

		colXY = colXY + s.color * 0.05f;
	}
	return colXY;
}

int main() {
	// ADD A CAMERA
	Camera camera = Camera(512, 512, 1000);

	// ADD SPHERES
	spheres.push_back(Sphere(Vector3(256, 10512, 0), 10000, Color(0, 0, 0))); // ground
	spheres.push_back(Sphere(Vector3(256, 256, 300), 100, true));
	spheres.push_back(Sphere(Vector3(175, 432, 222), 80, Color(0, 0, 255)));
	spheres.push_back(Sphere(Vector3(400, 175, 175), 40, Color(255, 255, 0)));

	// ADD LIGHTS
	lightsSources.push_back(Light(Vector3(216, 0, 222), Color(255, 255, 50), 400000));
	lightsSources.push_back(Light(Vector3(650, 200, 90), Color(0, 200, 255), 180000));
	lightsSources.push_back(Light(Vector3(-50, 150, 133), Color(240, 80, 0), 200000));

	image.resize(camera.width * camera.height * 4);

	for (unsigned x = 0; x < camera.width; x++) {
		for (unsigned y = 0; y < camera.height; y++) {
			int index = 4 * camera.width * y + 4 * x;

			Color colXY = Color(0, 0, 0);

			Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);

			Ray r = Ray(point, camera.GetNormalAtPoint(point));

			float distanceFirstSphere;
			int closestSphereIndex = hit_spheres(r, &distanceFirstSphere);

			if (closestSphereIndex != -1) { // There is an intersection with a sphere
				if (spheres[closestSphereIndex].isMirror == false) {
					Vector3 ptIntersection = Vector3(distanceFirstSphere * camera.GetNormalAtPoint(point).x + x + camera.position.x, distanceFirstSphere * camera.GetNormalAtPoint(point).y + y + camera.position.y, distanceFirstSphere * camera.GetNormalAtPoint(point).z + camera.position.z);
					colXY = manageLightReflection(ptIntersection, spheres[closestSphereIndex], index);
				} else {
					colXY = colXY + reflectRay(r, spheres[closestSphereIndex], point);
				}
			}

			setColor(image, index, colXY.Clamp255());
		}
	}

	encodeOneStep("image.png", image, camera.width, camera.height);
}