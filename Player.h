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
#include "PrimitiveDrawer.h"
#include "Bubble.h"

class Player {
  public:
	///初期化
	void Initialize(uint32_t textureHandle);

	///更新
	void Update();

	///描画
	void Draw(ViewProjection& viewProjection);

	//攻撃
	void Attack();

	Vector3 GetWorldPosition();
	Vector3 GetfrontVec();
	WorldTransform GetWorldTransform();
	float GetPlayerAngle();
	uint32_t GetLife();

	float Radian(float r);
	
	//当たり判定
	void OnCollision();
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//バブル
	Model* modelBubble_ = nullptr;
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t bubbleHandle_ = 0;

	Input* input_;
	DebugText* debugText_;

	/// <summary>
	/// キャラの向き(度数)
	/// </summary>
	float angle_ = 0.0f;

	Vector3 frontVec_;

	uint32_t life = 3;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//バブル
	std::list<std::unique_ptr<Bubble>> bubbles_;
};