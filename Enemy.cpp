#include "Enemy.h"

void (Enemy::*Enemy::phaseFuncTable[])() = {
  &Enemy::ApproachFunc,
  &Enemy::LeaveFunc,
};

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
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachFunc() {
	//移動
	worldTransform_.translation_.z -= 0.1f;
	//既定の位置まで来たら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveFunc() {
	//移動
	worldTransform_.translation_.z += 0.1f;
	if (worldTransform_.translation_.z > 10.0f) {
		phase_ = Phase::Approach;
	}
}
