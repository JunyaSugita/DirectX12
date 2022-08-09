#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "MatCalc.h"

enum class Phase {
	Approach, //�ڋ�
	Leave,    //���E
};

class Enemy {
  public:
	//������
	void Initialize(Model* model);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�s���p�^�[���֐�
	void ApproachFunc();
	void LeaveFunc();

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f���̃|�C���^
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*phaseFuncTable[])();
};
