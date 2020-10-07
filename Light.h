#pragma once

#include "lib/vector.h"
#include "color.h"

using namespace std;

class Light {
public:
	Vector3 position;
	Color color;
	float intensity;
	float radius;

	Light(Vector3 pos, Color c, float intensity, float radius);

	Color GetColor();
};