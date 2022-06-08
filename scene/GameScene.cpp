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

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	MatCalc(worldTransform_);

	//ビュープロジェクションの初期化
	//カメラ視点座標
	viewProjection_.eye = { 0, 50, -50 };

	//カメラの注視点座標を設定
	viewProjection_.target = { 0,0,0 };

	viewProjection_.Initialize();


	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	scene = 0;
	angle = 0;
}

void GameScene::Update() {
	debugCamera_->Update();

	if (scene == 0) {

		if (input_->TriggerKey(DIK_Q)) {
			scene = 1;
		}

		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.05f;

		//押した方向で移動ベクトル変更
		if (input_->PushKey(DIK_UP)) {
			move = { sin(worldTransform_.rotation_.y),0,cos(worldTransform_.rotation_.y) };
		}
		else if (input_->PushKey(DIK_DOWN)) {
			move = { -sin(worldTransform_.rotation_.y),0,-cos(worldTransform_.rotation_.y) };
		}

		//注視点移動(ベクトルの加算)
		worldTransform_.translation_ += move;

		//回転
		{
			//回転速さ
			const float RotSpeed = 0.05f;

			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_LEFT)) {
				worldTransform_.rotation_.y -= RotSpeed;
			}
			else if (input_->PushKey(DIK_RIGHT)) {
				worldTransform_.rotation_.y += RotSpeed;
			}

			if (worldTransform_.rotation_.y >= 6.2832f) {
				worldTransform_.rotation_.y -= 6.2832f;
			}
			if (worldTransform_.rotation_.y < 0) {
				worldTransform_.rotation_.y += 6.2832f;
			}
		}
	}
	else if(scene == 1) {
		if (input_->TriggerKey(DIK_Q)) {
			scene = 0;
		}
		//正面ベクトル
		Vector3 frontVec = viewProjection_.target - viewProjection_.eye;
		frontVec.y = 0;
		frontVec.normalize();
		//右ベクトル
		Vector3 rightVec = { 0,0,0 };
		Vector3 yVec = { 0,1,0 };

		rightVec = yVec.cross(frontVec);
		frontVec.normalize();
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		if (input_->PushKey(DIK_W)) {
			move += frontVec;
		}
		if (input_->PushKey(DIK_S)) {
			move += -frontVec;
		}
		if (input_->PushKey(DIK_A)) {
			move += -rightVec - frontVec;
		}
		if (input_->PushKey(DIK_D)) {
			move += rightVec + frontVec;
		}

		//注視点移動(ベクトルの加算)
		worldTransform_.translation_ = move;

		angle = 0;
		if (input_->PushKey(DIK_LEFT)) {
			angle--;
			if (angle < 0) {
				angle += 360;
			}
		}
		if (input_->PushKey(DIK_RIGHT)) {
			angle++;
			if (angle >= 360) {
				angle -= 360;
			}
		}
		
		viewProjection_.eye.y = cos(angle) * 50;
		viewProjection_.eye.z = sin(angle) * 50;
		viewProjection_.Initialize();

		debugText_->SetPos(0, 0);
		debugText_->Printf("move = %f,%f", move.x, move.z);
	}
	
	MatCalc(worldTransform_);
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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//for (int i = 0; i < 21; i++) {
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(100, -10, i * 10 - 100), Vector3(-100, -10, i * 10 - 100), Vector4(255, 0, 0, 255));
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i * 10 - 100, -10, 100), Vector3(i * 10 - 100, -10, -100), Vector4(0, 0, 255, 255));
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

float GameScene::Radian(float r) {
	return r * (PI / 180);
}