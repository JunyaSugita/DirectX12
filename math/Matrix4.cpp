#include "Matrix4.h"

Matrix4 Matrix4::operator*(const Matrix4& m2)const
{
    Matrix4 ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                ans.m[i][j] += m[i][k] * m2.m[k][j];
            }
        }
    }

    return ans;
}
