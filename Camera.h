#include <iostream>
#include "lib/vector.h"
#ifndef CAMERA_H
#define CAMERA_H

using namespace std;

class Camera {
public:
	int width, height, depth;
	Vector3 position, origin;
	Camera(int width, int height, int depth, Vector3 position = Vector3(0, 0, 0));
	Vector3 GetNormalAtPoint(Vector3 point);
};

#endif