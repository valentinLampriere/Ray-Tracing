#include "triangle.h"

Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3) {
	point1 = p1;
	point2 = p2;
	point3 = p3;
}

/*
From https://fr.wikipedia.org/wiki/Algorithme_d%27intersection_de_M%C3%B6ller%E2%80%93Trumbore
*/
bool Triangle::rayHit(Ray ray, float* distance) {
	const float EPSILON = 0.001f;
	Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = this->point2 - this->point1;
	edge2 = this->point3 - this->point1;

	h = Vector3::cross(ray.direction, edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // Le rayon est parallèle au triangle.
	f = 1.0 / a;
	s = ray.origin - this->point1;
	u = f * (s.dot(h));
	if (u < 0.0 || u > 1.0)
		return false;
	q = s.cross(edge1);
	v = f * ray.direction.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	// On calcule t pour savoir ou le point d'intersection se situe sur la ligne.
	float t = f * edge2.dot(q);
	if (t > EPSILON) { // Intersection avec le rayon
		*distance = t;
		return true;
	} else // On a bien une intersection de droite, mais pas de rayon.
		return false;
}