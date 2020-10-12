#include "box.h"

Box::Box() {
	coord1 = Vector3(-1, -1, -1);
	coord2 = Vector3(1, 1, 1);
}
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
	if (tmax < 0) {
		*t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		*t = tmax;
		return false;
	}

	*t = tmin;
	return true;
}

void Box::split(Box* b1, Box* b2) {
	float sizeX = this->coord2.x - this->coord1.x;
	float sizeY = this->coord2.y - this->coord1.y;
	float sizeZ = this->coord2.z - this->coord1.z;
	if (sizeX > sizeY && sizeX > sizeZ) {
		b1->coord1 = this->coord1;
		b1->coord2 = Vector3((this->coord1.x + this->coord2.x) / 2, this->coord2.y, this->coord2.z);
		b2->coord1 = Vector3((this->coord1.x + this->coord2.x) / 2, this->coord1.y, this->coord1.z);
		b2->coord2 = this->coord2;
	} else if (sizeY > sizeZ) {
		b1->coord1 = this->coord1;
		b1->coord2 = Vector3(this->coord2.x, (this->coord1.y + this->coord2.y) / 2, this->coord2.z);
		b2->coord1 = Vector3(this->coord1.x, (this->coord1.y + this->coord2.y) / 2, this->coord1.z);
		b2->coord2 = this->coord2;
	} else {
		b1->coord1 = this->coord1;
		b1->coord2 = Vector3(this->coord2.x, this->coord2.y, (this->coord1.z + this->coord2.z) / 2);
		b2->coord1 = Vector3(this->coord1.x, this->coord1.y, (this->coord1.z + this->coord2.z) / 2);
		b2->coord2 = this->coord2;
	}
}

void Box::settingSpheres(std::vector<Sphere> allSpheres) {
	/*for (int i = 0; i < allSpheres.size(); i++) {
		Sphere s = allSpheres[i];
		if (s.position.x  >= this->coord1.x &&
			s.position.y  >= this->coord1.y &&
			s.position.z  >= this->coord1.z &&
			s.position.x  <= this->coord2.x &&
			s.position.y  <= this->coord2.y &&
			s.position.z  <= this->coord2.z) {
			this->spheres.push_back(allSpheres[i]);
		}
	}*/
	for (int i = 0; i < allSpheres.size(); i++) {
		Sphere s = allSpheres[i];
		float dist_squared = s.radius * s.radius;
		if (s.position.x < this->coord1.x) dist_squared -= (s.position.x - this->coord1.x) * (s.position.x - this->coord1.x);
		else if (s.position.x > this->coord2.x) dist_squared -= (s.position.x - this->coord2.x) * (s.position.x - this->coord2.x);
		if (s.position.y < this->coord1.y) dist_squared -= (s.position.y - this->coord1.y) * (s.position.y - this->coord1.y);
		else if (s.position.y > this->coord2.y) dist_squared -= (s.position.y - this->coord2.y) * (s.position.y - this->coord2.y);
		if (s.position.z < this->coord1.z) dist_squared -= (s.position.z - this->coord1.z) * (s.position.z - this->coord1.z);
		else if (s.position.z > this->coord2.z) dist_squared -= (s.position.z - this->coord2.z) * (s.position.z - this->coord2.z);

		if(dist_squared > 0)
			this->spheres.push_back(allSpheres[i]);
	}	
}
bool Box::hasChildren() {
	return childBox1 != NULL && childBox2 != NULL;
}

void Box::print() {
	cout << "[" << this->spheres.size() << "] " << this->coord1 << " / " << this->coord2 << "\n";
}