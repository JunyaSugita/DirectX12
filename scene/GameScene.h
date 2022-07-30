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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//自キャラ
	//Player* player_ = nullptr;

	//テクスチャ
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//角度
	float angleX = 0;
	float angleY = 0;

	//カメラの種類
	int cameraNum = 0;

	WorldTransform worldTransform_;

	bool isHit;

	Sprite* sprite_ = nullptr;
	uint32_t scopeHandle_ = 0;

	//視点の正面ベクトル
	Vector3 frontVec;
	//対象とのベクトル
	Vector3 boxVec;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
