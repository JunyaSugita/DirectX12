#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0,10,100);
}

void Enemy::Update() { 
	worldTransform_.translation_.z -= 0.1f; 
	MatCalc(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
