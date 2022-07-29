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
	///初期化
	void Initialize(Model* model,uint32_t textureHandle);

	///更新
	void Update();

	///描画
	void Draw(ViewProjection& viewProjection);

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