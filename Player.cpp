#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};
	worldTransform_.scale_ = {0.4f, 0.8f, 0.4f};
	frontVec_ = {cos(Radian(angle_)), 0, sin(Radian(angle_))};
}

void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//移動速度
	const float kCharacterSpeed = 0.2f;

	if (worldTransform_.translation_.z < -21.3f) {
		angle_ += 0.02f;
	}
	if (worldTransform_.translation_.z > 20.0f) {
		angle_ += 0.02f;
	}
	if (angle_ >= 360.0f) {
		angle_ -= 360.0f;
	}
	frontVec_ = {cos(Radian(angle_)), 0, sin(Radian(angle_))};

	worldTransform_.translation_ += frontVec_ * 0.01f;

	//移動限界
	//const float kMoveLimitX = 34.0f;
	//const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	// worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	// worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//旋回
	Vector3 rotate = {0, 0, 0};

	//移動速度

	rotate = {0, -Radian(angle_), 0};
	worldTransform_.rotation_ = rotate;

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf(
	  "%f", angle_);

	//キャラクターの攻撃
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機の向きに合わせる
		velocity *= worldTransform_.matWorld_;

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

Vector3 Player::GetfrontVec() {
	Vector3 frontVec;
	frontVec = frontVec_;
	return frontVec;
}

float Player::Radian(float r) { return r * (3.14159265f / 180); }

void Player::OnCollision() {}
