#pragma once

#include "lib/vector.h"

using namespace std;

class Ray {
public:
	Vector3 origin;
	Vector3 direction;

	Ray(Vector3 o, Vector3 d);
};