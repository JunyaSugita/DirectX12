#pragma once
#include "EnemyBullet.h"
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "Player.h"

enum class Phase {
	Approach, //�ڋ�
	Leave,    //���E
};

class Enemy {
  public:
	//������
	void Initialize(Model* model,WorldTransform playerTransform);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�s���p�^�[���֐�
	void ApproachFunc();
	void LeaveFunc();

	//�e����
	void Fire(Vector3 transform);

	//�ڋ߃t�F�[�Y������
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	//�Q�b�^�[
	Vector3 GetWorldPosition();
	//�Z�b�^�[
	void SetWorldTransform(WorldTransform playerTransform);
	void SetPlayerAngle(float angle);
	void SetInput(bool is);

	//�����蔻��
	void OnCollision();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float Radian(float r);

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	WorldTransform playerTransform_;
	WorldTransform bulletCenter_;
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

	//���L����
	Player* player_ = nullptr;

	Vector3 playerPosition;
	Vector3 enemyPosition;
	float playerAngle_ = 0;

	bool a = true;

  public:
	//���ˊԊu
	static const int kFireInterval = 60 * 10;
};
