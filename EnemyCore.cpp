#include"EnemyCore.h"

void EnemyCore::Initialize(Vector3 transform,Model* model) { 
	assert(model);

	model_ = model;
	textur_ = TextureManager::Load("enemyBullet2.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform;
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	MatCalc(worldTransform_);
}

void EnemyCore::Update() {

}

void EnemyCore::Draw(ViewProjection viewProjection) {
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textur_);
	}
}

void EnemyCore::OnCollision() { isDead_ = true; }

Vector3 EnemyCore::GetWorldPos() {
	Vector3 a = worldTransform_.translation_;
	return a;
}