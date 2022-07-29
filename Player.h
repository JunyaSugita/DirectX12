#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
#include "MatCalc.h"
#include "ViewProjection.h"
#include "DebugCamera.h"

class Player {
  public:
	///������
	void Initialize(Model* model,uint32_t textureHandle);

	///�X�V
	void Update();

	///�`��
	void Draw(ViewProjection& viewProjection);

  private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;
};