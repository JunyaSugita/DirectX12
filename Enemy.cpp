#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0, 10, 100);
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
	default:
		//�ړ�
		worldTransform_.translation_.z -= 0.1f;
		//����̈ʒu�܂ŗ����痣�E
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		//�ړ�
		worldTransform_.translation_.z += 0.1f;
		if (worldTransform_.translation_.z > 10.0f) {
			phase_ = Phase::Approach;
		}
		break;
	}

	MatCalc(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
