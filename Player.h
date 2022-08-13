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
	///初期化
	void Initialize(Model* model,uint32_t textureHandle);

	///更新
	void Update();

	///描画
	void Draw(ViewProjection& viewProjection);

	//攻撃
	void Attack();

	Vector3 GetWorldPosition();
	
	//当たり判定
	void OnCollision();
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_;
	DebugText* debugText_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};