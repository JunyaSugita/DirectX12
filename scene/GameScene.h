#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
const float PI = 3.14159265f;


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public:
	enum PartId {
		kRoot,
		kSpine,
		kChest,
		kHead,
		kArmL,
		kArmR,
		kHip,
		kLegL,
		kLegR,

		kNumPartId
	};


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

	//テクスチャ
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	static const int MODEL_COUNT = 3;
	WorldTransform worldTransforms_[MODEL_COUNT];
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//角度
	float angle_ = 0;

	//カメラの種類
	int cameraNum_ = 0;

	//注視点カウント
	int targetCount_ = 0;

	//スコープモード
	bool scope_ = false;
	float scopeAngle_ = 0.5f;

	//スプライト
	Sprite* reticleSprite_ = nullptr;
	Sprite* scopeSprite_ = nullptr;
	uint32_t reticleHandle_ = 0;
	uint32_t scopeHandle_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
