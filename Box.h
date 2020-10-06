#include "lib/vector.h"

#ifndef BOX_H
#define BOX_H

using namespace std;

class Box {
public:
	Vector3 coord1, coord2;

	Box(Vector3 c1, Vector3 c2);
	static Box Union(const Box b1, const Box b2) {
		Vector3 min = Vector3::min(b1.coord1, b2.coord1);
		Vector3 max = Vector3::max(b1.coord1, b2.coord1);
		return Box(min, max);
	}
	Box Union(Box b);
};

#endif