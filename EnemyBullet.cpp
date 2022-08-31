#include "EnemyBullet.h"

void EnemyBullet::Initialize(
  Model* model, Model* model2, WorldTransform playerTransform, Vector3 transform, int type) {
	// NULLポインタチェック
	assert(model);
	assert(model2);

	model_ = model;
	model2_ = model2;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");
	deadHandle_ = TextureManager::Load("enemyBullet2.png");

	worldTransform_.Initialize();
	//引数で受け取った変数をセット
	worldTransform_.translation_ = playerTransform.translation_;
	worldTransform_.translation_ += transform;
	MatCalc(worldTransform_);

	transform_ = transform;
	type_ = type;
}

void EnemyBullet::Update() {
	if (type_ == 0) {
		if (isHit_ == true && isBreak_ == false) {
			// Break
			std::unique_ptr<Break> newBreak = std::make_unique<Break>();
			newBreak->Inisialize(model_, textureHandle_, worldTransform_);

			break_.push_back(std::move(newBreak));
			isBreak_ = true;
		}
		if (isHit_ == false) {
			if (isMove_ == false) {
				worldTransform_.translation_ = bulletCenter_.translation_;
				worldTransform_.translation_ +=
				  {cos(Radian(playerAngle_)) * transform_.x, transform_.y,
				   sin(Radian(playerAngle_)) * transform_.x};
				worldTransform_.rotation_ = {0, -Radian(playerAngle_), 0};
				MatCalc(worldTransform_);
			}
			if (isMove_ == true) {
				//座標を移動
				worldTransform_.translation_ += velocity2_;
				//モデルの回転
				worldTransform_.rotation_.x -= 0.005f;
				worldTransform_.rotation_.y -= 0.005f;

				MatCalc(worldTransform_);
				if (lenTimer_ > 2) {
					lenTimer_--;
				}
			}
		}
	} else {
		velocity_.y = 0.0f;
		if (worldTransform_.translation_.y > -0.0f) {
			worldTransform_.translation_.y -= 0.1f;
		}
		if (isHit_ == true && isBreak_ == false) {
			// Break
			std::unique_ptr<Break> newBreak = std::make_unique<Break>();
			newBreak->Inisialize(model_, textureHandle_, worldTransform_);

			break_.push_back(std::move(newBreak));
			isBreak_ = true;
		}
		if (isHit_ == false) {
			if (isMove_ == false) {
				worldTransform_.translation_ = bulletCenter_.translation_;
				worldTransform_.translation_ +=
				  {cos(Radian(playerAngle_)) * transform_.x, transform_.y,
				   sin(Radian(playerAngle_)) * transform_.x};
				worldTransform_.rotation_ = {0, -Radian(playerAngle_), 0};
				MatCalc(worldTransform_);
			}
			if (isMove_ == true) {
				//座標を移動
				worldTransform_.translation_ += velocity_;
				//モデルの回転
				worldTransform_.rotation_.x -= velocity_.x / 10;
				worldTransform_.rotation_.z -= velocity_.z / 10;

				MatCalc(worldTransform_);
			}
		}
	}
	for (std::unique_ptr<Break>& bubble : break_) {
		bubble->Update();
	}
	if (worldTransform_.translation_.y <= -3.0f) {
		isDead_ = true;
	}
	//時間経過でデス
	if (isHit_ == true) {
		if (--deathTimer_ <= 0) {
			isDead_ = true;
		}
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	if (isHit_ == false) {
		if (type_ == 1) {
			model2_->Draw(worldTransform_, viewProjection);
		} else {
			model_->Draw(worldTransform_, viewProjection, textureHandle_);
		}
	}

	for (std::unique_ptr<Break>& break1 : break_) {
		break1->Draw(viewProjection);
	}
}

void EnemyBullet::Move() { isMove_ = true; }

Vector3 EnemyBullet::GetWorldPos() {
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

Vector3 EnemyBullet::GetBulletVec() {
	Vector3 enebulletVec;
	enebulletVec = bulletVec_;
	return enebulletVec;
}

bool EnemyBullet::GetisMove() { return isMove_; }

bool EnemyBullet::GetisHit() { return isHit_; }

uint32_t EnemyBullet::GetTimer() { return lenTimer_; }

void EnemyBullet::SetBulletVec(Vector3 Vec) { bulletVec_ = Vec; }

void EnemyBullet::SetVelocity(Vector3 velocity) { velocity_ = velocity; }

void EnemyBullet::SetVelocity2(Vector3 velocity) { velocity2_ = velocity; }

void EnemyBullet::SetisHit(bool isHit) { isHit_ = isHit; }

void EnemyBullet::SetWorldTransform(WorldTransform worldTransform) {
	bulletCenter_ = worldTransform;
}

void EnemyBullet::SetPlayerAngle(float angle) { playerAngle_ = angle; }

void EnemyBullet::BulletVecNormalize() { bulletVec_.normalize(); }

void EnemyBullet::OnCollision() { 
	//isDead_ = true; 
	isHit_ = true;
	//Break
	std::unique_ptr<Break> newBreak = std::make_unique<Break>();
	newBreak->Inisialize(model_, textureHandle_, worldTransform_);

	break_.push_back(std::move(newBreak));
	for (std::unique_ptr<Break>& bubble : break_) {
		bubble->Update();
	}
}

float EnemyBullet::Radian(float r) { return r * (3.14159265f / 180); }