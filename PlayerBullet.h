#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "MatCalc.h"

class PlayerBullet {
  public:
	//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f���̃|�C���^
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};
