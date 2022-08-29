#pragma once
#include "EnemyBullet.h"
#include "MatCalc.h"
#include "Model.h"
#include "Player.h"
#include "ViewProjection.h"
#include <cassert>
#include "EnemyCore.h"

enum class Phase {
	Approach, //接近
	Leave,    //離脱
};

class Enemy {
  public:
	//初期化
	void Initialize(Model* model, WorldTransform playerTransform, Vector3 playerFrontVec);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//行動パターン関数
	void ApproachFunc();
	void LeaveFunc();

	//弾発射
	void Fire(Vector3 transform, int type);

	//接近フェーズ初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	//ゲッター
	Vector3 GetWorldPosition();
	int GetCoolTime();
	//セッター
	void SetWorldTransform(WorldTransform playerTransform);
	void SetPlayerAngle(float angle);
	void SetInput(int num);
	void SetCoolTime(int num);

	//当たり判定
	void OnCollision();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float Radian(float r);

  private:
	//ワールド変換
	WorldTransform worldTransform_;
	WorldTransform playerTransform_;
	WorldTransform bulletCenter_;
	//モデルのポインタ
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
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
	float playerAngle_ = 0;
	Vector3 playerFrontVec_;

	bool a = true;

	/// <summary>
	/// 体力
	/// </summary>
	int hp = 4;
	/// <summary>
	/// クールタイム
	/// </summary>
	int coolTime = 15 * 60;

	//コア
	EnemyCore* enemyCore_[4];

  public:
	//発射間隔
	static const int kFireInterval = 60 * 10;
};
