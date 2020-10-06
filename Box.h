#include "lib/vector.h"

#ifndef BOX_H
#define BOX_H

using namespace std;

class Box {
public:
	Vector3 coord1, coord2;

	Box(Vector3 c1, Vector3 c2);
};

#endif