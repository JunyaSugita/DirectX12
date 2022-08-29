#include"EnemyCore.h"

void EnemyCore::Initialize(Vector3 transform,Model* model) { 
	assert(model);

	model_ = model;
	textur_ = TextureManager::Load("enemyBullet2.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform;
	MatCalc(worldTransform_);
}

void EnemyCore::Update() {

}

void EnemyCore::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textur_);
}