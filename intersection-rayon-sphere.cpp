#include <iostream>
#include <optional>
#include <map>
#include "Sphere.h"
#include "Box.h"
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
std::default_random_engine generator(10000);

float random() {
	std::uniform_real_distribution<> dist(-1, 1);
	return dist(generator);
	//return dist(e2); -- Non seeded
}
float random(float min, float max) {
	std::uniform_real_distribution<> dist(min, max);
	return dist(generator);
	//return dist(e2); -- Non seeded
}

int hit_spheres(Ray ray, float* distance) {
	int closeSphereIndex = -1;
	for (int i = 0; i < spheres.size(); i ++) {
		float d;
		if (spheres[i].rayHit(ray, &d)) {
			if (closeSphereIndex == -1) {
				closeSphereIndex = i;
				*distance = d;
			} else {
				if (d < *distance) {
					closeSphereIndex = i;
					*distance = d;
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
		//float dist_otherSphere = hit_sphere(_aSphere, _r);
		float dist;
		if (_aSphere.rayHit(_r, &dist) && dist * dist < distance2) { // If the ray hits another sphere
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
				Vector3 offset = Vector3(random(), random(), random()) * aLight.position * 0.4f;
				colorALight = colorALight + calcLuminosityAtPoint(pointIntersection, s, aLight.position + offset, aLight.GetColor());
			}
			colorALight = colorALight / nbRay;
			colXY = colXY + colorALight;

			Vector3 dir = (aLight.position - pointIntersection).normalized();

			Ray _r = Ray(pointIntersection + dir * -0.05f, dir);
			for (Sphere& _aSphere : spheres) {
				if (_aSphere != s) {
					//float dist_otherSphere = hit_sphere(_aSphere, _r);
					float dist;
					if (_aSphere.rayHit(_r, &dist)) { // If the ray hits another sphere
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

Box generateSpheres(Vector3 origin, float width, float height, int amountOfSpheres) {
	Vector3 minCoord = Vector3(width / 2 + origin.x, height / 2 + origin.y, 200);
	Vector3 maxCoord = Vector3(-width / 2 + origin.x, -height / 2 + origin.y, 0);
	for (int i = 0; i < amountOfSpheres; i++) {
		float radius = random(5, 25);
		float x = random(-width/2 + origin.x, width/2 + origin.x);
		float y = random(-height/2 + origin.y, height/2 + origin.y);
		float z = random(radius, 200);
		minCoord = Vector3::min(minCoord, Vector3(x, y, z));
		maxCoord = Vector3::max(maxCoord, Vector3(x, y, z));
		spheres.push_back(Sphere(Vector3(x, y, z), radius));
	}
	return Box(minCoord, maxCoord);
}


int main() {
	// ADD A CAMERA
	Camera camera = Camera(512, 512, 500, Vector3(256,256,0));

	// ADD SPHERES
	/*
	spheres.push_back(Sphere(Vector3(512, 101024, 0), 100000, Color(255, 200, 0))); // Ground
	spheres.push_back(Sphere(Vector3(101050, 512, 5000), 100000, Color(215, 205, 210))); // Wall right
	spheres.push_back(Sphere(Vector3(512, 512, 101200), 100000, Color(215, 205, 210))); // Wall back
	
	spheres.push_back(Sphere(Vector3(512, 512, 350), 200, true));
	spheres.push_back(Sphere(Vector3(600, 512, -1050), 1000, true));
	spheres.push_back(Sphere(Vector3(750, 864, 800), 160, Color(0, 255, 0)));
	spheres.push_back(Sphere(Vector3(350, 864, 300), 160, Color(0, 0, 255)));
	spheres.push_back(Sphere(Vector3(650, 900, 150), 100, true));
	spheres.push_back(Sphere(Vector3(800, 350, 300), 80, Color(255, 255, 0)));
	*/

	map<int, Box> boxes;
	Box b = generateSpheres(camera.origin, camera.width, camera.height, 10000);

	b.settingSpheres(spheres);

	// ADD LIGHTS
	lightsSources.push_back(Light(Vector3(200, -200, 400), Color(255, 255, 255), 2000000, 200.0f));
	lightsSources.push_back(Light(Vector3(-100, 300, -150), Color(240, 80, 0), 750000, 1.0f));

	image.resize(camera.width * camera.height * 4);

	for (unsigned x = 0; x < camera.width; x++) {
		for (unsigned y = 0; y < camera.height; y++) {

			int index = 4 * camera.width * y + 4 * x;
			int nbRay = 10;
			Color colXY = Color(0, 0, 0);
			
			Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);
			Ray r = Ray(point, (point - camera.origin).normalized());

			for (int i = 0; i < spheres.size(); i++) {
				float d;
				Vector3 coord1;
				Vector3 coord2;
				spheres[i].getBoxCoord(&coord1, &coord2);
				Box box = Box(coord1, coord2);
				//box.settingSpheres(spheres);
				if (box.rayHit(r, &d)) {
					colXY = Color(d, d, d);
				}
			}

			/*
			for (int i = 0; i < nbRay; i++) {
				Vector3 offset = Vector3(random() * 0.01f, random() * 0.01f, random() * 0.01f);
				Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);

				Ray r = Ray(point + offset, (point + offset - camera.origin).normalized());

				float distanceFirstSphere;
				int closestSphereIndex = hit_spheres(r, &distanceFirstSphere);

				if (closestSphereIndex != -1) { // There is an intersection with a sphere
					Vector3 ptIntersection = Vector3(distanceFirstSphere * r.direction.x + x + camera.position.x, distanceFirstSphere * r.direction.y + y + camera.position.y, distanceFirstSphere * r.direction.z + camera.position.z);
					colXY = colXY + manageLightReflection(point, ptIntersection, spheres[closestSphereIndex], index);
				}
			}
			colXY = colXY / nbRay;
			*/
			

			setColor(image, index, colXY.Clamp255());
		}
	}

	encodeOneStep("image.png", image, camera.width, camera.height);
}