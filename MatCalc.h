#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "math.h"

void SetMatScale(Matrix4& matScale, const Vector3& scale);

void SetMatTrans(Matrix4& matTrans, const Vector3& trans);

void SetMatRot(Matrix4& matRot, const Vector3& rot, const char way);

void MatCalc(WorldTransform& worldtransform);