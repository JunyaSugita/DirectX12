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

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet> & bullet) { 
		return bullet->IsDead(); 
	});

	//�ړ�
	Vector3 move = {0, 0, 0};

	//�ړ����x
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move += {-kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_D)) {
		move += {kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_W)) {
		move += {0, kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_S)) {
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

	//����
	Vector3 rotate = {0, 0, 0};

	//�ړ����x
	const float krotateSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		rotate += {0, -krotateSpeed, 0};
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotate += {0, krotateSpeed, 0};
	}
	if (input_->PushKey(DIK_UP)) {
		rotate += {-krotateSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_DOWN)) {
		rotate += {krotateSpeed, 0, 0};
	}
	worldTransform_.rotation_ += rotate;

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf(
		"%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);

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
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos; 
}

void Player::OnCollision() {
	
}
