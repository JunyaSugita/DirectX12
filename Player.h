#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
#include "MatCalc.h"
#include "ViewProjection.h"
#include "DebugCamera.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "PrimitiveDrawer.h"
#include "Bubble.h"

class Player {
  public:
	///������
	void Initialize(uint32_t textureHandle);

	///�X�V
	void Update();

	///�`��
	void Draw(ViewProjection& viewProjection);

	//�U��
	void Attack();

	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform();
	float GetPlayerAngle();
	uint32_t GetLife();
	Vector3 GetFrontVec();
	int GetType() { return type; };

	void SetCameraVec(Vector3 vec) { cameraVec_ = vec; }
	void SetType(int i) { type = i; }

	float Radian(float r);
	
	//�����蔻��
	void OnCollision();
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�o�u��
	Model* modelBubble_ = nullptr;
	
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	uint32_t bubbleHandle_ = 0;

	Input* input_;
	DebugText* debugText_;

	/// <summary>
	/// �L�����̌���(�x��)
	/// </summary>
	float angle_ = 0.0f;
	float saveAngle_ = 0.0f;
	Vector3 cameraVec_;

	Vector3 frontVec_;

	uint32_t life = 3;

	//�ˌ��^�C�v
	int type = 0;

	//�W�����v�^�C�}�[
	int jumpTimer = 0;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//�o�u��
	std::list<std::unique_ptr<Bubble>> bubbles_;


};