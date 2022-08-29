#include "Enemy.h"

void (Enemy::*Enemy::phaseFuncTable[])() = {
  &Enemy::ApproachFunc,
  &Enemy::LeaveFunc,
};

void Enemy::Initialize(Model* model, WorldTransform playerTransform, Vector3 playerFrontVec) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.png");
	//モデル読み込み
	bulletModel_ = Model::CreateFromOBJ("enemyBullet", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {20.0f, 30.0f, 150.0f};

	bulletCenter_.Initialize();

	playerTransform_ = playerTransform;
	playerFrontVec_ = playerFrontVec;

	ApproachInitialize();

	for (int i = 0; i < 4;i++) {
		enemyCore_[i] = new EnemyCore;
	}
	enemyCore_[0]->Initialize(Vector3(0, 100, 0),model_);
	enemyCore_[1]->Initialize(Vector3(100, 100, 100), model_);
	enemyCore_[2]->Initialize(Vector3(100, 100, 0), model_);
	enemyCore_[3]->Initialize(Vector3(0, 100, 100), model_);
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);

	bulletCenter_.translation_ = playerTransform_.translation_;
	bulletCenter_.rotation_ = playerTransform_.rotation_;
	bulletCenter_.translation_ +=
	  {cos(Radian(playerAngle_ - 90)) * -60, 8, sin(Radian(playerAngle_ - 90)) * -60};
	MatCalc(bulletCenter_);

	playerPosition = playerTransform_.translation_;

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		//弾の速度
		float kBulletSpeed = 0.15f;

		Vector3 playerLen = playerPosition - bullet->GetWorldPos();
		playerLen.normalize();

		uint32_t len = bullet->GetTimer();

		if (len > 1500) {
			len /= 10;
		} else if (len > 1250) {
			len = 20;
		} else {
			len = 0;
		}

		Vector3 playerLen2 =
		  (playerPosition +
		   (Vector3(cos(Radian(playerAngle_)), 0, sin(Radian(playerAngle_))) * len)) -
		  bullet->GetWorldPos();
		playerLen2.normalize();

		playerLen *= kBulletSpeed;
		playerLen2 *= kBulletSpeed;

		bullet->SetVelocity(playerLen);
		bullet->SetVelocity2(playerLen2);
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

	for (EnemyCore* enemyCore : enemyCore_) {
		enemyCore->Draw(viewProjection);
	}
}

void Enemy::ApproachFunc() {
	coolTime--;
	if (hp == 4 && coolTime == 0) {
		Fire(Vector3(0, 0, 0), 0);
		Fire(Vector3(2, 0, 0), 0);
		Fire(Vector3(-2, 0, 0), 0);
	}
	if (hp == 3 && coolTime == 0) {
		Fire(Vector3(-2, 0, 0), 0);
		Fire(Vector3(6, 0, 0), 1);
		Fire(Vector3(2, 0, 0), 0);
		Fire(Vector3(-6, 0, 0), 1);
	}
	if (hp == 2 && coolTime == 0) {
		Fire(Vector3(-3, -1, 0), 1);
		Fire(Vector3(3, -1, 0), 1);
		Fire(Vector3(0, 1, 0), 0);
		Fire(Vector3(3, 1, 0), 0);
		Fire(Vector3(0, -1, 0), 1);
		Fire(Vector3(-3, 1, 0), 0);
	}
	if (hp == 1 && coolTime == 0) {
		Fire(Vector3(-6, 1, 0), 0);
		Fire(Vector3(-3, 1, 0), 0);
		Fire(Vector3(0, 1, 0), 0);
		Fire(Vector3(3, 1, 0), 0);
		Fire(Vector3(6, 1, 0), 0);
		Fire(Vector3(-6, -1, 0), 1);
		Fire(Vector3(-3, -1, 0), 1);
		Fire(Vector3(0, -1, 0), 1);
		Fire(Vector3(3, -1, 0), 1);
		Fire(Vector3(6, -1, 0), 1);
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

void Enemy::Fire(Vector3 transform, int type) {
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, bulletModel_, bulletCenter_, transform, type);

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

int Enemy::GetCoolTime() { return coolTime; }

void Enemy::SetWorldTransform(WorldTransform playerTransform) {
	playerTransform_ = playerTransform;
}

void Enemy::SetPlayerAngle(float angle) { playerAngle_ = angle; }

void Enemy::SetInput(int num) { hp += num; }

void Enemy::SetCoolTime(int num) { coolTime = num; }

void Enemy::OnCollision() {}

float Enemy::Radian(float r) { return r * (3.14159265f / 180); }
