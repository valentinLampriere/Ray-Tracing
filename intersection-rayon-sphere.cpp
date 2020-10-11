#include <iostream>
#include <optional>
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
	//return dist(e2); // Non-seeded
}
float random(float min, float max) {
	std::uniform_real_distribution<> dist(min, max);
	return dist(generator);
	//return dist(e2); // Non-seeded
}

int hit_spheres(Ray ray, std::vector<Sphere> spheres, float* distance) {
	int closeSphereIndex = -1;
	for (int i = 0; i < spheres.size(); i++) {
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

/*Box* hit_boxes(Ray ray, Box b1, Box b2) {
	int closeCubeIndex = -1;
	float minDistance = 99999;
	float distance;

	bool hitBox1 = false;

	if (b1.rayHit(ray, &distance)) {
		minDistance = distance;
		hitBox1 = true;
	}
	if (b2.rayHit(ray, &distance)) {
		if (minDistance < distance) {
			return &b2;
		}
	}
	if (hitBox1) {
		return &b1;
	}
	return NULL;
}*/

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

Color manageLightReflection(Vector3 pointOrigin, Vector3 pointIntersection, std::vector<Sphere> spheres, Sphere s, int index);

Color reflectRay(Ray ray, std::vector<Sphere> spheres, Sphere sphere, Vector3 point, int index, int depth = 0) {
	
	if (depth >= 10) {
		return Color(0, 0, 0);
	}

	Vector3 normale = sphere.normaleAtPoint(point);
	Vector3 directionNewRay = (normale * 2 * Vector3::dot(Vector3() - ray.direction, normale) + ray.direction).normalized();
	Ray newRay = Ray(point + directionNewRay, directionNewRay);
	float distance;
	int indexSphere = hit_spheres(newRay, spheres , &distance);
	if (indexSphere == -1) {
		return Color(0, 0, 0);
	} else {
		if (spheres[indexSphere].isMirror) {
			return reflectRay(newRay, spheres, spheres[indexSphere], point + directionNewRay * distance + directionNewRay, index, depth + 1);
		}
		else {
			return manageLightReflection(ray.origin, point + directionNewRay * distance, spheres, spheres[indexSphere], index);
		}
	}
}

Color manageLightReflection(Vector3 pointOrigin, Vector3 pointIntersection, std::vector<Sphere> spheres, Sphere s, int index) {
	Color colXY = Color(0, 0, 0);

	if (s.isMirror == false) {
		int nbRay = 100;
		for (Light& aLight : lightsSources) {

			Color colorALight = Color();
			for (int i = 0; i < nbRay; i++) {
				Vector3 offset = Vector3(0, 0, 0);
				if(nbRay > 1)
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
		colXY = colXY + reflectRay(ray, spheres, s, pointIntersection, index);
	}
	return colXY;
}

Box* generateSpheres(Vector3 origin, float width, float height, int amountOfSpheres) {
	Vector3 minCoord = Vector3(width / 2 + origin.x, height / 2 + origin.y, 500);
	Vector3 maxCoord = Vector3(-width / 2 + origin.x, -height / 2 + origin.y, 0);
	for (int i = 0; i < amountOfSpheres; i++) {
		float radius = random(20, 40);
		//float x = random(-width/2 + origin.x, width/2 + origin.x);
		//float y = random(-height/2 + origin.y, height/2 + origin.y);
		float x = random(-width / 2 + origin.x, 0);
		float y = random(-height/2 + origin.y, 0);
		float z = random(radius * 2, 300);

		float R = random(0, 255);
		float G = random(0, 255);
		float B = random(0, 255);

		minCoord = Vector3::min(minCoord, Vector3(x - radius, y - radius, z - radius));
		maxCoord = Vector3::max(maxCoord, Vector3(x + radius, y + radius, z + radius));
		spheres.push_back(Sphere(Vector3(x, y, z), radius, Color(R, G, B)));
	}
	return new Box(minCoord, maxCoord);
}

void populateBoxes(Box* aBox) {
	aBox->settingSpheres(spheres);
	if (aBox->spheres.size() <= 5) {
		return;
	}

	Box* b1 = new Box();
	Box* b2 = new Box();
	aBox->split(b1, b2);

	aBox->childBox1 = b1;
	aBox->childBox2 = b2;

	populateBoxes(aBox->childBox1);
	populateBoxes(aBox->childBox2);
}

void findBoxes(Ray r, Box* b, vector<pair<Box*, float>>* boxes) {
	float distance;
	if (!b->hasChildren()) {
		if (b->spheres.size() > 0) {
			boxes->push_back(make_pair(b, distance));
		}
	}
	if (b->hasChildren() && b->rayHit(r, &distance)) {
		findBoxes(r, b->childBox1, boxes);
		findBoxes(r, b->childBox2, boxes);
	}
}

void printScene(Box box, int depth = 0) {
	for (int i = 0; i < depth; i++) {
		cout << " | ";
	}
	box.print();
	if (box.hasChildren()) {
		printScene(*box.childBox1, depth + 1);
		printScene(*box.childBox2, depth + 1);
	}
}

int main() {
	// ADD A CAMERA
	Camera camera = Camera(512, 512, 500, Vector3(256,256,0));


	// ADD LIGHTS
	lightsSources.push_back(Light(Vector3(200, -200, 400), Color(255, 255, 255), 2000000, 200.0f));
	lightsSources.push_back(Light(Vector3(-100, 300, -150), Color(240, 80, 0), 750000, 1.0f));


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

	Box *b = generateSpheres(camera.origin, camera.width, camera.height, 100);
	populateBoxes(b);

	image.resize(camera.width * camera.height * 4);

	printScene(*b);


	for (unsigned x = 0; x < camera.width; x++) {
		for (unsigned y = 0; y < camera.height; y++) {

			int index = 4 * camera.width * y + 4 * x;
			int nbRay = 100;
			Color colXY = Color(0, 0, 0);

			vector<pair<Box*, float>> boxes;
			Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);
			Ray r = Ray(point, (point - camera.origin).normalized());

			findBoxes(r, b, &boxes);
			
			if (boxes.size() > 0) {
				pair<Box*, float> closestBox = boxes[0];
				float distanceFirstSphere;
				Sphere* closestSphere = NULL;
				for (int i = 0; i < boxes.size(); i++) {
					if (closestBox.second < boxes[i].second) {
						closestBox = boxes[i];
						int closestSphereIndex = hit_spheres(r, closestBox.first->spheres, &distanceFirstSphere);
						if (closestSphereIndex != -1) {
							closestSphere = &closestBox.first->spheres[closestSphereIndex];
						}
					}
				}
				if (closestSphere != NULL) {
					Vector3 ptIntersection = Vector3(distanceFirstSphere * r.direction.x + x + camera.position.x, distanceFirstSphere * r.direction.y + y + camera.position.y, distanceFirstSphere * r.direction.z + camera.position.z);
					colXY = colXY + manageLightReflection(point, ptIntersection, closestBox.first->spheres, *closestSphere, index);
				} else
					colXY = colXY + Color(0, 0, 0);
			} else
				colXY = colXY + Color(0, 0, 0);
			/*for (int i = 0; i < nbRay; i++) {
				Vector3 offset = Vector3(random() * 0.01f, random() * 0.01f, random() * 0.01f);
				Ray r = Ray(point + offset, (point + offset - camera.origin).normalized());

				for (int i = 0; i < boxes.size(); i++) {
					int indexCube = hit_cubes(r);
					if (indexCube != -1) {
						float distanceFirstSphere;
						int closestSphereIndex = hit_spheres(r, boxes[i].spheres, &distanceFirstSphere);

						if (closestSphereIndex != -1) { // There is an intersection with a sphere
							Vector3 ptIntersection = Vector3(distanceFirstSphere * r.direction.x + x + camera.position.x, distanceFirstSphere * r.direction.y + y + camera.position.y, distanceFirstSphere * r.direction.z + camera.position.z);
							colXY = colXY + manageLightReflection(point, ptIntersection, boxes[i].spheres, boxes[i].spheres[closestSphereIndex], index);
						} else
							colXY = colXY + Color(0, 0, 0);
					} else
						colXY = colXY + Color(0, 0, 0);
				}
			}*/
			
			/*for (int i = 0; i < nbRay; i++) {
				Vector3 offset = Vector3(random() * 0.01f, random() * 0.01f, random() * 0.01f);
				Vector3 point = Vector3(camera.position.x + x, camera.position.y + y, camera.position.z);

				Ray r = Ray(point + offset, (point + offset - camera.origin).normalized());

				float distanceFirstSphere;
				int closestSphereIndex = hit_spheres(r, spheres, &distanceFirstSphere);

				if (closestSphereIndex != -1) { // There is an intersection with a sphere
					Vector3 ptIntersection = Vector3(distanceFirstSphere * r.direction.x + x + camera.position.x, distanceFirstSphere * r.direction.y + y + camera.position.y, distanceFirstSphere * r.direction.z + camera.position.z);
					colXY = colXY + manageLightReflection(point, ptIntersection, spheres, spheres[closestSphereIndex], index);
				}
			}*/

			//colXY = colXY / nbRay;

			setColor(image, index, colXY.Clamp255());
		}
	}

	encodeOneStep("image.png", image, camera.width, camera.height);
}