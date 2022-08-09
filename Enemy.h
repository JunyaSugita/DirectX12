#pragma once
#include "EnemyBullet.h"
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>

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

	//�e����
	void Fire();

	//�ڋ߃t�F�[�Y������
	void ApproachInitialize();

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f���̃|�C���^
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*phaseFuncTable[])();

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���˃^�C�}�[
	int32_t fireTimer = 0;

  public:
	//���ˊԊu
	static const int kFireInterval = 60;
};
