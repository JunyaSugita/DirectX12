#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Update() {
	Vector3 move = {0, 0, 0};

	//�ړ����x
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

	//�ړ����E
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//�͈͂𒴂��Ȃ�����
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