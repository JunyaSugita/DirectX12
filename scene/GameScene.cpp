#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲 (回転費用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//親
	worldTransform_[0].Initialize();

	//カメラ視点座標
	viewProjection_.eye = { 0, 50, -50 };

	//カメラの注視点座標を設定
	viewProjection_.target = { 0,0,0 };

	viewProjection_.Initialize();
}

void GameScene::Update() {
	//キャラクターの移動ベクトル
	XMFLOAT3 move = { 0,0,0 };

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.05f;

	//押した方向で移動ベクトル変更
	if (input_->PushKey(DIK_UP)) {
		move = { sin(worldTransform_[0].rotation_.y),0,cos(worldTransform_[0].rotation_.y)};
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { -sin(worldTransform_[0].rotation_.y),0,-cos(worldTransform_[0].rotation_.y) };
	}

	//注視点移動(ベクトルの加算)
	worldTransform_[0].translation_.x += move.x;
	worldTransform_[0].translation_.y += move.y;
	worldTransform_[0].translation_.z += move.z;

	worldTransform_[0].UpdateMatrix();

	//回転
	{
		//回転速さ
		const float RotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			worldTransform_[0].rotation_.y -= RotSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			worldTransform_[0].rotation_.y += RotSpeed;
		}

		if (worldTransform_[0].rotation_.y >= 6.2832f) {
			worldTransform_[0].rotation_.y -= 6.2832f;
		}
		if (worldTransform_[0].rotation_.y < 0) {
			worldTransform_[0].rotation_.y += 6.2832f;
		}
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"eye:(%f)", worldTransform_[0].rotation_.y
	);

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

	model_->Draw(worldTransform_[0], viewProjection_, textureHandle_);
	
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
