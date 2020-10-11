#pragma once

#include "Triangle.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class Mesh {
public:
	vector<Triangle> triangles;
	static Mesh loadSphere(int nbMeridian, int nbParallels, float radius);
	static Mesh loadOff(string filename);
};