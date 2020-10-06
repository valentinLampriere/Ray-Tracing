#include "box.h"

Box::Box(Vector3 c1, Vector3 c2) {
	coord1 = c1;
	coord2 = c2;
}

Box Box::Union(Box b) {
	return Box::Union(*this, b);
}