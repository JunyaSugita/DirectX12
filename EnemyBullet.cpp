#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");

	worldTransform_.Initialize();
	//引数で受け取った変数をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	//座標を移動
	worldTransform_.translation_ += velocity_;

	MatCalc(worldTransform_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPos() { 
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void EnemyBullet::OnCollision() { 
	isDead_ = true; 
}