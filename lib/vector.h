#pragma once

#include <iostream>
#include <math.h>
#include <string>
#include <random>

using namespace std;

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float vectorX, float vectorY, float vectorZ);
	float lengthSquare();
	float length();
	static float dot(Vector3 v1, float f) {
		return v1.x * f + v1.y * f + v1.z * f;
	}
	float dot(float f);
	static float dot(Vector3 v1, Vector3 v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	float dot(Vector3 v2);
	static Vector3 cross(Vector3 v1, Vector3 v2) {
		Vector3 vector;
		vector.x = v1.y * v2.z - v1.z * v2.y;
		vector.y = v1.z * v2.x - v1.x * v2.z;
		vector.z = v1.x * v2.y - v1.y * v2.x;
		return vector;
	}
	Vector3 cross(Vector3 v);
	static Vector3 normalize(Vector3 v1) {
		try {
			return v1 / v1.length();
		}
		catch (string err) {
			cout << err + "\n";
		}
	}
	Vector3 normalized();

	static Vector3 min(Vector3 v1, Vector3 v2) {
		float x = std::min(v1.x, v2.x);
		float y = std::min(v1.y, v2.y);
		float z = std::min(v1.z, v2.z);
		//if (v1.lengthSquare() <= v2.lengthSquare())
		//	return v1;
		//return v2;
		return Vector3(x, y, z);
	}
	Vector3 min(const Vector3 v);
	static Vector3 max(Vector3 v1, Vector3 v2) {
		float x = std::max(v1.x, v2.x);
		float y = std::max(v1.y, v2.y);
		float z = std::max(v1.z, v2.z);
		//if (v1.lengthSquare() <= v2.lengthSquare())
		//	return v2;
		//return v1;
		return Vector3(x, y, z);
	}
	Vector3 max(const Vector3 v);
	Vector3 operator+(const Vector3 v);
	Vector3 operator+(float f);
	Vector3 operator-(const Vector3 v);
	Vector3 operator-(float f);
	Vector3 operator/(const float f);
	Vector3 operator*(const float f);
	Vector3 operator*(const Vector3 v);
	friend std::ostream& operator<<(std::ostream& output, const Vector3& vector) {
		output << "(" << vector.x << " ; " << vector.y << " ; " << vector.z << ")";
		return output;
	}
};