#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};

