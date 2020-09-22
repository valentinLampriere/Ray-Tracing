#include "lib/vector.h"
#include "color.h"

#ifndef SPHERE_H
#define SPHERE_H

using namespace std;

class Sphere {
public:
	Vector3 position;
	float radius;
	Color color;
	bool isMirror;

	Sphere() = default;
	Sphere(Vector3 o, float r, bool isMirror, Color c = Color(1, 1, 1));
	friend std::ostream& operator<<(std::ostream& output, const Sphere& sphere) {
		output << "Sphere(" << sphere.position << " / " << sphere.radius << ")";
		return output;
	}
};

#endif