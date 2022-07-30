#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black1x1.png");

	worldTransform_.Initialize();
	//�����Ŏ󂯎�����ϐ����Z�b�g
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	//���W���ړ�
	worldTransform_.translation_ += velocity_;

	MatCalc(worldTransform_); 

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
