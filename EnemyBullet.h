#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>

class EnemyBullet {
  public:
	//������
	void Initialize(Model* model, WorldTransform playerTransform, Vector3 transform);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�e�𓮂���
	void Move();

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }
	Vector3 GetWorldPos();
	Vector3 GetBulletVec();
	bool GetisMove();
	bool GetisHit();

	//�Z�b�^�[
	void SetBulletVec(Vector3 Vec);
	void SetVelocity(Vector3 velocity);
	void SetisHit(bool isHit);
	void SetWorldTransform(WorldTransform worldTransform);
	void SetPlayerAngle(float angle);

	//�m�[�}���C�Y
	void BulletVecNormalize();

	//�����蔻��
	void OnCollision();

	float Radian(float r);

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	WorldTransform bulletCenter_;
	Vector3 transform_;
	//���f���̃|�C���^
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
	uint32_t deadHandle_ = 0;

	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 300;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
	//�ړ��t���O
	bool isMove_ = false;

	//���C����t���O
	bool isHit_ = false;

	Vector3 bulletVec_;

	float playerAngle_;
};