#pragma once
#include "EnemyBullet.h"
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "Player.h"

enum class Phase {
	Approach, //接近
	Leave,    //離脱
};

class Enemy {
  public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update(Vector3 playerPos);

	//描画
	void Draw(const ViewProjection& viewProjection);

	//行動パターン関数
	void ApproachFunc();
	void LeaveFunc();

	//弾発射
	void Fire();

	//接近フェーズ初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//当たり判定
	void OnCollision();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
  private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*phaseFuncTable[])();

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

	Vector3 playerPosition;
	Vector3 enemyPosition;

  public:
	//発射間隔
	static const int kFireInterval = 60 * 10;
};
