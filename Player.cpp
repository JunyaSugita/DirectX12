#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};
	worldTransform_.scale_ = {0.4f, 0.8f, 0.4f};
	frontVec_ = {cos(Radian(angle_)), 0, sin(Radian(angle_))};
}

void Player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//�ړ����x
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

	//�ړ����E
	//const float kMoveLimitX = 34.0f;
	//const float kMoveLimitY = 18.0f;

	//�͈͂𒴂��Ȃ�����
	// worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	// worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	// worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	// worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//����
	Vector3 rotate = {0, 0, 0};

	//�ړ����x

	rotate = {0, -Radian(angle_), 0};
	worldTransform_.rotation_ = rotate;

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf(
	  "%f", angle_);

	//�L�����N�^�[�̍U��
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���@�̌����ɍ��킹��
		velocity *= worldTransform_.matWorld_;

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//�e��o�^����
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
