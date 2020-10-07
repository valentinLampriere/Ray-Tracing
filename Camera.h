#pragma once

#include <iostream>
#include "lib/vector.h"

using namespace std;

class Camera {
public:
	int width, height, depth;
	Vector3 position, origin;
	Camera(int width, int height, int depth, Vector3 position = Vector3(0, 0, 0));
	Vector3 GetNormalAtPoint(Vector3 point);
};