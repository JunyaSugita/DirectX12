#include "Player.h"

void Player::Initialize(uint32_t textureHandle) {

	model_ = Model::CreateFromOBJ("player", true);
	modelBubble_ = Model::Create();
	textureHandle_ = textureHandle;
	bubbleHandle_ = TextureManager::Load("bubble.png");

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, -300.0f};
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	frontVec_ = {cos(Radian(angle_)), 0, sin(Radian(angle_))};
}

void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });
	bubbles_.remove_if([](std::unique_ptr<Bubble>& bubble) { return bubble->IsDead(); });

	//移動速度
	const float kCharacterSpeed = 0.1f;
	if (type == 0) {
		angle_ = saveAngle_;
		worldTransform_.translation_.y = 0;
		if (worldTransform_.rotation_.x > 0) {
			worldTransform_.rotation_.x -= 0.004f;
		}
		if (angle_ < 90 || worldTransform_.translation_.z < -150.0f) {
			angle_ += 0.03f;
			if (worldTransform_.rotation_.x < 0.5f) {
				worldTransform_.rotation_.x += 0.005f;
			}
		}
		if (worldTransform_.translation_.z > 150.0f) {
			angle_ += 0.03f;
			if (worldTransform_.rotation_.x < 0.5f) {
				worldTransform_.rotation_.x += 0.005f;
			}
		}
		if (angle_ >= 360.0f) {
			angle_ -= 360.0f;
		}
		saveAngle_ = angle_;
		jumpTimer = 0;
	}
	if (type == 1) {
		angle_ += 5.0f;
		jumpTimer++;
		if (worldTransform_.translation_.y <= 300 && jumpTimer >= 500) {
			worldTransform_.translation_.y += 0.1f;
		}
	}
	if (worldTransform_.translation_.y == 0) {
		frontVec_ = {cos(Radian(angle_)), 0, sin(Radian(angle_))};
		worldTransform_.translation_ += frontVec_ * kCharacterSpeed;
	}

	MatCalc(worldTransform_);

	//移動限界
	// const float kMoveLimitX = 34.0f;
	// const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	// worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	// worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//旋回
	float rotate = 0;

	//移動速度

	rotate = -Radian(angle_);
	worldTransform_.rotation_.y = rotate;

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf("%f", angle_);

	//キャラクターの攻撃
	if (type == 1) {
		Attack();
	}

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//バブル
	std::unique_ptr<Bubble> newBubble = std::make_unique<Bubble>();
	newBubble->Inisialize(modelBubble_, bubbleHandle_, worldTransform_);

	bubbles_.push_back(std::move(newBubble));
	for (std::unique_ptr<Bubble>& bubble : bubbles_) {
		bubble->Update();
	}
}

void Player::taoreru() { worldTransform_.rotation_.x -= 0.001; }

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	if (worldTransform_.translation_.y == 0) {
		for (std::unique_ptr<Bubble>& bubble : bubbles_) {
			bubble->Draw(viewProjection);
		}
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 0.1f;
		cameraVec_ *= kBulletSpeed;

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(modelBubble_, worldTransform_.translation_, cameraVec_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

uint32_t Player::GetLife() { return life; }

Vector3 Player::GetFrontVec() { return frontVec_; }

WorldTransform Player::GetWorldTransform() { return worldTransform_; }

float Player::GetPlayerAngle() { return angle_; }

float Player::Radian(float r) { return r * (3.14159265f / 180); }

void Player::OnCollision() { life--; }
