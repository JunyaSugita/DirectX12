#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>

class EnemyBullet {
  public:
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//ゲッター
	bool IsDead() const { return isDead_; }
	Vector3 GetWorldPos();

	//当たり判定
	void OnCollision();



  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 20;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};