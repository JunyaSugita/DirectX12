#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>

class EnemyBullet {
  public:
	//初期化
	void Initialize(Model* model, WorldTransform playerTransform, Vector3 transform);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//弾を動かす
	void Move();

	//ゲッター
	bool IsDead() const { return isDead_; }
	Vector3 GetWorldPos();
	Vector3 GetBulletVec();
	bool GetisMove();
	bool GetisHit();

	//セッター
	void SetBulletVec(Vector3 Vec);
	void SetVelocity(Vector3 velocity);
	void SetisHit(bool isHit);
	void SetWorldTransform(WorldTransform worldTransform);
	void SetPlayerAngle(float angle);

	//ノーマライズ
	void BulletVecNormalize();

	//当たり判定
	void OnCollision();

	float Radian(float r);

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	WorldTransform bulletCenter_;
	Vector3 transform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t deadHandle_ = 0;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 300;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	//移動フラグ
	bool isMove_ = false;

	//レイ判定フラグ
	bool isHit_ = false;

	Vector3 bulletVec_;

	float playerAngle_;
};