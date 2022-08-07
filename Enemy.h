#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "MatCalc.h"

class Enemy {
  public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
