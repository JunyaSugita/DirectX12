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
	// delete player_;
	delete sprite_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_[0] = TextureManager::Load("white1x1.png");
	textureHandle_[1] = TextureManager::Load("white1x1.png");
	textureHandle_[2] = TextureManager::Load("white1x1.png");
	// 3Dモデルの生成
	model_ = Model::Create();

	worldTransforms_[0].translation_ = {5, 0, -30};
	worldTransforms_[1].translation_ = {0, 7, 0};
	worldTransforms_[2].translation_ = {-5, -5, -20};

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
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	isHit_ = false;

	scopeHandle_ = TextureManager::Load("red10x10.png");
	sprite_ = Sprite::Create(scopeHandle_, {635, 355});
	//自キャラ生成
	// player_ = new Player();
	//自キャラの初期化
	// player_->Initialize(model_, textureHandle_);
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

	/*if (isDebugCameraActive_) {
	    debugCamera_->Update();
	    viewProjection_ = debugCamera_->GetViewProjection();
	    viewProjection_.TransferMatrix();
	} else {
	    viewProjection_.TransferMatrix();
	}*/

	//注視点移動(ベクトルの加算)
	float angleSpeed = 0.2f;

	if (input_->PushKey(DIK_RIGHT)) {
		angleX += angleSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		angleX -= angleSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		angleY += angleSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		angleY -= angleSpeed;
	}

	viewProjection_.target.x = angleX;
	viewProjection_.target.y = angleY;
	viewProjection_.UpdateMatrix();

	//視点の正面ベクトルの更新
	for (int i = 0; i < 3; i++) {
		frontVecs_[i] = {
		  viewProjection_.target.x - viewProjection_.eye.x,
		  viewProjection_.target.y - viewProjection_.eye.y,
		  viewProjection_.target.z - viewProjection_.eye.z
		};

		frontVecs_[i].normalize();
	}

	

	for (int i = 0; i < 3; i++) {
		boxVecs_[i] = {
		  worldTransforms_[i].translation_.x - viewProjection_.eye.x,
		  worldTransforms_[i].translation_.y - viewProjection_.eye.y,
		  worldTransforms_[i].translation_.z - viewProjection_.eye.z
		};

		boxVecs_[i].normalize();
	}

	

	float zNums_[3];
	for (int i = 0; i < 3; i++) {
		zNums_[i] = worldTransforms_[i].translation_.z - viewProjection_.eye.z;
	}

	for (int i = 0; i < 3; i++) {
		frontVecs_[i] *= zNums_[i];
		boxVecs_[i] *= zNums_[i];
	}

	Vector3 hitChecks_[3];

	for (int i = 0; i < 3; i++) {
		hitChecks_[i].x = frontVecs_[i].x - boxVecs_[i].x;
		hitChecks_[i].y = frontVecs_[i].y - boxVecs_[i].y;
		hitChecks_[i].z = frontVecs_[i].z - boxVecs_[i].z;
	}
	for (int i = 0; i < 3; i++) {
		if (
			hitChecks_[i].x <= 1.0f && hitChecks_[i].x >= -1.0f &&
			hitChecks_[i].y <= 1.0f &&hitChecks_[i].y >= -1.0f &&
			hitChecks_[i].z <= 1.0f && hitChecks_[i].z >= -1.0f) {
			debugText_->SetPos(0, 20 * i);
			debugText_->Printf("Hit!");
			textureHandle_[i] = TextureManager::Load("ray.jpg");
		} 
		else {
			textureHandle_[i] = TextureManager::Load("white1x1.png");
		}
	}

	//自キャラ更新
	// player_->Update();
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
	// player_->Draw(viewProjection_);

	for (int i = 0; i < 3;i++) {
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_[i]);
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
	sprite_->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

float GameScene::Radian(float r) { return r * (PI / 180); }