#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
const float PI = 3.14159265f;

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	      /// <summary>
	      /// コンストクラタ
	      /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	float Radian(float r);

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollision();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	//テクスチャ
	uint32_t textureHandle_ = 0;
	uint32_t waterHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_[5];
	uint32_t cameraNum_ = 0;

	float cameraAngleX = 300.0f;
	float cameraAngleY = 0.0f;
	float cameraSpeed = 0.1f;

	//弾発射の間隔
	const int32_t moveCoolTime = 5 * 60;
	int32_t moveCoolTimer = moveCoolTime;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//角度
	float angle = 0;

	//カメラベクトル
	Vector3 cameraFrontVec;

	//カメラ時間
	const uint32_t cameraTime = 5 * 60;
	uint32_t cameraTimer = cameraTime;

	//スプライト
	Sprite* sprite_ = nullptr;
	uint32_t scopeHandle_ = 0;
	Sprite* lifeSprite_[3];
	uint32_t lifeHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
	uint32_t titleHandle_ = 0;
	Sprite* startSprite_ = nullptr;
	uint32_t startHandle_ = 0;
	Sprite* goTitleSprite_ = nullptr;
	uint32_t goTitleHandle_ = 0;
	Sprite* goRetrySprite_ = nullptr;
	uint32_t goRetryHandle_ = 0;
	Sprite* gameClearSprite_ = nullptr;
	uint32_t gameClearHandle_ = 0;
	Sprite* gameoverSprite_ = nullptr;
	uint32_t gameoverHandle_ = 0;
	Sprite* setumeiSprite_ = nullptr;
	uint32_t setumeiHandle_ = 0;
	Sprite* setumei2Sprite_ = nullptr;
	uint32_t setumei2Handle_ = 0;

	//サウンド
	uint32_t gameBGM_ = 0;
	uint32_t iceBreakSE_ = 0;

	//水
	WorldTransform water_;

	//死カウント
	const uint32_t deathTime = 30 * 60;
	uint32_t deathTimer = deathTime;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	enum Scene{
		title,
		main,
		gameCrear,
		gameover,
	};

	int scene = title;
};
