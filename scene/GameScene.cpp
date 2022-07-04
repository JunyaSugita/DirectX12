#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "random"
#include "MatCalc.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
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
	//3Dモデルの生成
	model_ = Model::Create();

	worldTransforms_[PartId::kRoot].Initialize();

	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0,2.0f,0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];

	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0,2.0f,0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];

	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { 2.0f,0,0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];

	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { -2.0f,0,0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];

	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0,0,0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];

	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { 2.0f,-2.0f,0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];

	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { -2.0f,-2.0f,0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];

	for (WorldTransform& worldTransform : worldTransforms_) {
		MatCalc(worldTransform);
	}

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());


}

void GameScene::Update() {
	debugCamera_->Update();

	//キャラクターの移動
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 1.0f;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x += 1.0f;
		}

		worldTransforms_[PartId::kRoot].translation_ += move;

		//キャラクターの腕回転ベクトル
		Vector3 Arot = { 0,0,0 };
		if (input_->PushKey(DIK_Q)) {
			Arot.y -= 1.0f;
		}
		else if (input_->PushKey(DIK_E)) {
			Arot.y += 1.0f;
		}

		worldTransforms_[PartId::kChest].rotation_ += Arot;

		//キャラクターの足回転ベクトル
		Vector3 Lrot = { 0,0,0 };
		if (input_->PushKey(DIK_A)) {
			Lrot.y -= 1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			Lrot.y += 1.0f;
		}

		worldTransforms_[PartId::kHip].rotation_ += Lrot;

		for (int i = 0; i < 9; i++) {
			MatCalc(worldTransforms_[i]);
		}
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
	for (int i = 0; i < 9; i++) {
		if (i == 0 || i == 1) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	for (int i = 0; i < 21; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(100, 0, i * 10 - 100), Vector3(-100, 0, i * 10 - 100), Vector4(255, 0, 0, 255));
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i * 10 - 100, 0, 100), Vector3(i * 10 - 100, 0, -100), Vector4(0, 0, 255, 255));
	}
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

float GameScene::Radian(float r) {
	return r * (PI / 180);
}