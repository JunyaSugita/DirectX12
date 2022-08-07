#include "GameScene.h"
#include "AxisIndicator.h"
#include "MatCalc.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "random"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> rot(0.0f, PI * 2);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化

	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラ生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラ生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_ = debugCamera_->GetViewProjection();
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.TransferMatrix();
	}

	//自キャラ更新
	player_->Update();

	//敵更新
	if (enemy_ != NULL) {
		enemy_->Update();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//自キャラの更新
	player_->Draw(viewProjection_);

	//敵
	if (enemy_ != NULL) {
		enemy_->Draw(viewProjection_);
	}

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	// for (int i = 0; i < 21; i++) {
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(100, 0, i * 10 - 100), Vector3(-100, 0, i
	//* 10 - 100), Vector4(255, 0, 0, 255)); 	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i
	//* 10 - 100, 0, 100), Vector3(i * 10 - 100, 0, -100), Vector4(0, 0, 255, 255));
	//}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

float GameScene::Radian(float r) { return r * (PI / 180); }