#include "box.h"

Box::Box(Vector3 c1, Vector3 c2) {
	coord1 = c1;
	coord2 = c2;
}

Box Box::Union(Box b) {
	return Box::Union(*this, b);
}

bool Box::rayHit(Ray ray, float* t) {
	Vector3 dirfrac = Vector3(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
	float t1 = (this->coord1.x - ray.origin.x) * dirfrac.x;
	float t2 = (this->coord2.x - ray.origin.x) * dirfrac.x;
	float t3 = (this->coord1.y - ray.origin.y) * dirfrac.y;
	float t4 = (this->coord2.y - ray.origin.y) * dirfrac.y;
	float t5 = (this->coord1.z - ray.origin.z) * dirfrac.z;
	float t6 = (this->coord2.z - ray.origin.z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		*t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		*t = tmax;
		return false;
	}

	*t = tmin;
	return true;
}