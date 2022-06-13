#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "MatCalc.h"

/// <summary>
/// ���L����
/// </summary>
class Player{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
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

