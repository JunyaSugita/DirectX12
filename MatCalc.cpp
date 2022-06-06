#include "MatCalc.h"


void SetMatScale(Matrix4& matScale, const Vector3& scale) {
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
};

void SetMatTrans(Matrix4& matTrans, const Vector3& trans) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				matTrans.m[i][j] = 1;
			}
			else {
				matTrans.m[i][j] = 0;
			}
		}
	}

	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

void SetMatRot(Matrix4& matRot, const Vector3& rot, const char way) {
	
	if (way == 'z') {
		//Z軸回転
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = cos(rot.z);
		matRot.m[0][1] = sin(rot.z);
		matRot.m[1][0] = -sin(rot.z);
		matRot.m[1][1] = cos(rot.z);
	}

	if (way == 'x') {
		//X軸回転
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[1][1] = cos(rot.x);
		matRot.m[1][2] = sin(rot.x);
		matRot.m[2][1] = -sin(rot.x);
		matRot.m[2][2] = cos(rot.x);
	}
	//Y軸回転
	if (way == 'y') {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRot.m[i][j] = 1;
				}
				else {
					matRot.m[i][j] = 0;
				}
			}
		}
		matRot.m[0][0] = cos(rot.y);
		matRot.m[0][2] = -sin(rot.y);
		matRot.m[2][0] = sin(rot.y);
		matRot.m[2][2] = cos(rot.y);
	}
}

void MatCalc(WorldTransform& worldTransform) {
	//ワールドトランスフォームの初期化
	worldTransform.Initialize();

	Matrix4 matScale;
	SetMatScale(matScale, worldTransform.scale_);

	Matrix4 matTrans;
	SetMatTrans(matTrans, worldTransform.translation_);

	Matrix4 matRotZ;
	SetMatRot(matRotZ, worldTransform.rotation_, 'z');

	Matrix4 matRotX;
	SetMatRot(matRotX, worldTransform.rotation_, 'x');

	Matrix4 matRotY;
	SetMatRot(matRotY, worldTransform.rotation_, 'y');

	Matrix4 matRot = matRotZ * matRotX * matRotY;

	//単位行列を代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				worldTransform.matWorld_.m[i][j] = 1;
			}
			else {
				worldTransform.matWorld_.m[i][j] = 0;
			}
		}
	}

	worldTransform.matWorld_ *= matScale * matRot * matTrans;


	//行列の転送
	worldTransform.TransferMatrix();
}