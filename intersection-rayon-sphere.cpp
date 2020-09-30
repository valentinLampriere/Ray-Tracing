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

std::random_device rd;
std::mt19937 e2(rd());

//unsigned width = 512, height = 512;

float random() {
	std::uniform_real_distribution<> dist(-1, 1);
	return dist(e2);
}

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
Color calcLuminosityAtPoint(Vector3 point, Sphere s, Vector3 lightPosition, Color lightColor) {
	float V = 1;
	Vector3 N = (point - s.position).normalized(); // Normale on "point"

	Vector3 dir = (lightPosition - point).normalized();

	Ray _r = Ray(point + dir * 0.05f, dir);

	float distance2 = (lightPosition - point).lengthSquare();

	for (Sphere& _aSphere : spheres) {
		float dist_otherSphere = hit_sphere(_aSphere, _r);
		
		if (dist_otherSphere >= 0 && dist_otherSphere * dist_otherSphere < distance2) { // If the ray hits another sphere
			return Color(0,0,0);
		}
	}
	return (lightColor * std::abs(N.dot(dir))) / (distance2 * M_PI);
}

Color manageLightReflection(Vector3 pointOrigin, Vector3 pointIntersection, Sphere s, int index);

Color reflectRay(Ray ray, Sphere sphere, Vector3 point, int index, int depth = 0) {
	
	if (depth >= 10) {
		return Color(0, 0, 0);
	}

	Vector3 normale = sphere.normaleAtPoint(point);
	Vector3 directionNewRay = (normale * 2 * Vector3::dot(Vector3() - ray.direction, normale) + ray.direction).normalized();
	Ray newRay = Ray(point + directionNewRay, directionNewRay);
	float distance;
	int indexSphere = hit_spheres(newRay, &distance);
	if (indexSphere == -1) {
		return Color(0, 0, 0);
	} else {
		if (spheres[indexSphere].isMirror) {
			return reflectRay(newRay, spheres[indexSphere], point + directionNewRay * distance + directionNewRay, index, depth + 1);
		}
		else {
			return manageLightReflection(ray.origin, point + directionNewRay * distance, spheres[indexSphere], index);
		}
	}
}

Color manageLightReflection(Vector3 pointOrigin, Vector3 pointIntersection, Sphere s, int index) {
	Color colXY = Color(0, 0, 0);

	if (s.isMirror == false) {
		int nbRay = 5;
		for (Light& aLight : lightsSources) {

			Color colorALight = Color();
			for (int i = 0; i < nbRay; i++) {
				//cout << random() << "\n";
				Vector3 offset = Vector3(random(), random(), random()) * aLight.position * 0.4f;
				colorALight = colorALight + calcLuminosityAtPoint(pointIntersection, s, aLight.position + offset, aLight.GetColor());
			}
			colorALight = colorALight / nbRay;
			colXY = colXY + colorALight;
			
			//colXY = colXY + calcLuminosityAtPoint(pointIntersection, s, aLight.position, aLight.GetColor());

			Vector3 dir = (aLight.position - pointIntersection).normalized();

			Ray _r = Ray(pointIntersection + dir * -0.05f, dir);
			for (Sphere& _aSphere : spheres) {
				if (_aSphere != s) {
					float dist_otherSphere = hit_sphere(_aSphere, _r);
					if (dist_otherSphere >= 0) { // If the ray hits another sphere
						colXY = colXY + Color(0, 0, 0);
						//setColor(image, index, Color(0, 0, 0));
					}
				}
			}
		}
		colXY = colXY + s.color * 0.1f;
	} else {
		Ray ray = Ray(pointOrigin, (pointIntersection - pointOrigin).normalized());
		colXY = colXY + reflectRay(ray, s, pointIntersection, index);
	}
	return colXY;
}

int main() {
	// ADD A CAMERA
	Camera camera = Camera(1024, 1024, 2000);

	// ADD SPHERES
	spheres.push_back(Sphere(Vector3(512, 101024, 0), 100000, Color(255, 200, 0))); // Ground
	spheres.push_back(Sphere(Vector3(101050, 512, 5000), 100000, Color(215, 205, 210))); // Wall right
	spheres.push_back(Sphere(Vector3(512, 512, 101200), 100000, Color(215, 205, 210))); // Wall back

	spheres.push_back(Sphere(Vector3(512, 512, 350), 200, true));
	spheres.push_back(Sphere(Vector3(600, 512, -1050), 1000, true));
	spheres.push_back(Sphere(Vector3(750, 864, 800), 160, Color(0, 255, 0)));
	spheres.push_back(Sphere(Vector3(350, 864, 300), 160, Color(0, 0, 255)));
	spheres.push_back(Sphere(Vector3(650, 900, 150), 100, true));
	spheres.push_back(Sphere(Vector3(800, 350, 300), 80, Color(255, 255, 0)));

	// ADD LIGHTS
	lightsSources.push_back(Light(Vector3(200, -200, 400), Color(255, 255, 255), 2000000, 200.0f));
	//lightsSources.push_back(Light(Vector3(1250, 500, 150), Color(0, 200, 255), 800000));
	lightsSources.push_back(Light(Vector3(-100, 300, -150), Color(240, 80, 0), 750000, 1.0f));

	image.resize(camera.width * camera.height * 4);

	for (unsigned x = 0; x < camera.width; x++) {
		for (unsigned y = 0; y < camera.height; y++) {
			int index = 4 * camera.width * y + 4 * x;
			int nbRay = 5;
			Color colXY = Color(0, 0, 0);

			for (int i = 0; i < nbRay; i++) {
				//Vector3 offset = Vector3(random(), random(), random());
				Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);

				//Ray r = Ray(point, camera.GetNormalAtPoint(point));
				Ray r = Ray(point, (point - camera.origin).normalized());

				float distanceFirstSphere;
				int closestSphereIndex = hit_spheres(r, &distanceFirstSphere);

				if (closestSphereIndex != -1) { // There is an intersection with a sphere
					Vector3 ptIntersection = Vector3(distanceFirstSphere * r.direction.x + x + camera.position.x, distanceFirstSphere * r.direction.y + y + camera.position.y, distanceFirstSphere * r.direction.z + camera.position.z);
					colXY = colXY + manageLightReflection(point, ptIntersection, spheres[closestSphereIndex], index);
				}
			}
			colXY = colXY / nbRay;
			setColor(image, index, colXY.Clamp255());
		}
	}

	encodeOneStep("image.png", image, camera.width, camera.height);
}