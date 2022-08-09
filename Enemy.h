#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "MatCalc.h"

enum class Phase {
	Approach, //接近
	Leave,    //離脱
};

class Enemy {
  public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//行動パターン関数
	void ApproachFunc();
	void LeaveFunc();

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*phaseFuncTable[])();
};
