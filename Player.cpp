#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Update() {
	Vector3 move = {0, 0, 0};

	//移動速度
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move += {-kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move += {kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_UP)) {
		move += {0, kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_DOWN)) {
		move += {0, -kCharacterSpeed, 0};
	}
	worldTransform_.translation_ += move;

	//移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX); 
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX); 
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY); 

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf("%f,%f,%f", worldTransform_.translation_);
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}