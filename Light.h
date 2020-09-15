#include "lib/vector.h"

#ifndef LIGHT_H
#define LIGHT_H

using namespace std;

class Light {
public:
	Vector3 position;
	int r, g, b;

	Light(Vector3 pos, int r, int g, int b);
};

#endif