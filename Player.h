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
	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;
};

