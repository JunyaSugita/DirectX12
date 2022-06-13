#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "MatCalc.h"

class Player{
public:
	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw();

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

