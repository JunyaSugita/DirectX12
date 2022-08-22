#include "Enemy.h"

void (Enemy::*Enemy::phaseFuncTable[])() = {
  &Enemy::ApproachFunc,
  &Enemy::LeaveFunc,
};

void Enemy::Initialize(Model* model, WorldTransform playerTransform) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {20.0f, 30.0f, 150.0f};

	bulletCenter_.Initialize();

	playerTransform_ = playerTransform;

	ApproachInitialize();
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);

	bulletCenter_.translation_ = playerTransform_.translation_;
	bulletCenter_.rotation_ = playerTransform_.rotation_;
	bulletCenter_.translation_ += {cos(Radian(playerAngle_ - 90)) * -60, 8, sin(Radian(playerAngle_ - 90)) * -60};
	MatCalc(bulletCenter_);

	playerPosition = playerTransform_.translation_;

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		//弾の速度
		const float kBulletSpeed = 0.15f;

		Vector3 playerLen = playerPosition - bullet->GetWorldPos();
		playerLen.normalize();
		playerLen *= kBulletSpeed;

		bullet->SetVelocity(playerLen);
		bullet->SetWorldTransform(bulletCenter_);
		bullet->SetPlayerAngle(playerAngle_);

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
	if (a == false) {
		Fire(Vector3(-2, 0, 0));
		Fire(Vector3(2, 0, 0));
		Fire(Vector3(-2, 4, 0));
		Fire(Vector3(2, 4, 0));
		Fire(Vector3(-6, 0, 0));
		Fire(Vector3(6, 0, 0));
		Fire(Vector3(-6, 4, 0));
		Fire(Vector3(6, 4, 0));
		a = true;
	}

	//既定の位置まで来たら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveFunc() {
	//移動
	// worldTransform_.translation_.z += 0.5f;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire(Vector3 transform) {
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_,bulletCenter_, transform);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Enemy::SetWorldTransform(WorldTransform playerTransform) {
	playerTransform_ = playerTransform;
}

void Enemy::SetPlayerAngle(float angle) { playerAngle_ = angle; }

void Enemy::SetInput(bool is) { a = is; }

void Enemy::OnCollision() {}

float Enemy::Radian(float r) { return r * (3.14159265f / 180); }
