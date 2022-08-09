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

	ApproachInitialize();
}

void Enemy::Update() { 
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachFunc() {
	//移動
	worldTransform_.translation_.z -= 0.1f;

	//タイマーカウントダウン
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	//既定の位置まで来たら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveFunc() {
	//移動
	worldTransform_.translation_.z += 0.5f;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire() {
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//自機の向きに合わせる
	velocity *= worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}