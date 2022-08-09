#include "Enemy.h"

void (Enemy::*Enemy::phaseFuncTable[])() = {
  &Enemy::ApproachFunc,
  &Enemy::LeaveFunc,
};

void Enemy::Initialize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0, 10, 100);

	ApproachInitialize();
}

void Enemy::Update() { 
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachFunc() {
	//�ړ�
	worldTransform_.translation_.z -= 0.1f;

	//�^�C�}�[�J�E���g�_�E��
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	//����̈ʒu�܂ŗ����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveFunc() {
	//�ړ�
	worldTransform_.translation_.z += 0.5f;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//���@�̌����ɍ��킹��
	velocity *= worldTransform_.matWorld_;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize() {
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}