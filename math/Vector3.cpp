#include "Vector3.h"
#include <math.h>

float Vector3::length() const { return sqrt((x * x) + (y * y) + (z * z)); }

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= length();
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }

Vector3 Vector3::cross(const Vector3& v) const {
	return Vector3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y - v.x));
}

Vector3& Vector3::operator*=(const Matrix4& m2) {
	Vector3 ans;

	ans.x = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0];
	ans.y = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1];
	ans.z = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2];

	*this = ans;
	return *this;
}