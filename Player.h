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

class Player {
  public:
	///������
	void Initialize(Model* model,uint32_t textureHandle);

	///�X�V
	void Update();

	///�`��
	void Draw(ViewProjection& viewProjection);

	//�U��
	void Attack();

	Vector3 GetWorldPosition();
	
	//�����蔻��
	void OnCollision();
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};