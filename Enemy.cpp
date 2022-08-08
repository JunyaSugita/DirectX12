#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0, 10, 100);
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
	default:
		//移動
		worldTransform_.translation_.z -= 0.1f;
		//既定の位置まで来たら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		//移動
		worldTransform_.translation_.z += 0.1f;
		if (worldTransform_.translation_.z > 10.0f) {
			phase_ = Phase::Approach;
		}
		break;
	}

	MatCalc(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
