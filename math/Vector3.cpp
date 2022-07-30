#include "Vector3.h"

Vector3& Vector3::operator*=(const Matrix4& m2) {
	Vector3 ans;

	ans.x = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0];
	ans.y = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1];
	ans.z = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2];

	*this = ans;
	return *this;
}