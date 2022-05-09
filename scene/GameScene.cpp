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

	//動かすやつ
	worldTransform_[0].Initialize();

	//動かさないやつ
	worldTransform_[1].translation_ = { 0,0,10 };
	worldTransform_[1].Initialize();
	worldTransform_[2].translation_ = { -10,0,-10 };
	worldTransform_[2].Initialize();
	worldTransform_[3].translation_ = { 10,0,-10 };
	worldTransform_[3].Initialize();

	for (int i = 4; i < 100; i++) {
		worldTransform_[i].translation_ = { 0,0,0 };
		worldTransform_[i].scale_ = { 0.3f,0.3f,0.3f };
		bullet[i].isShot = 0;
		bullet[i].moveX = 0;
		bullet[i].moveY = 0;
		bullet[i].moveZ = 0;
		worldTransform_[i].Initialize();
	}

	//カメラ視点座標
	viewRadius = {
		0,
		50,
		-50
	};

	viewProjection_.eye = { viewRadius.x, viewRadius.y, viewRadius.z };

	//カメラの注視点座標を設定
	viewProjection_.target = { 0,0,0 };

	viewProjection_.Initialize();
}

void GameScene::Update() {
	//キャラクターの移動ベクトル
	XMFLOAT3 move = { 0,0,0 };

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.5f;

	//押した方向で移動ベクトル変更
	if (input_->PushKey(DIK_UP)) {
		move = { sin(worldTransform_[0].rotation_.y) * kCharacterSpeed,0,cos(worldTransform_[0].rotation_.y) * kCharacterSpeed };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { -sin(worldTransform_[0].rotation_.y) * kCharacterSpeed,0,-cos(worldTransform_[0].rotation_.y) * kCharacterSpeed };
	}

	//移動(ベクトルの加算)
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

		//カメラ移動

		viewProjection_.eye.x = worldTransform_[0].translation_.x +
			-sin(worldTransform_[0].rotation_.y) * sqrt(viewRadius.x * viewRadius.x + viewRadius.y * viewRadius.y + viewRadius.z * viewRadius.z);

		viewProjection_.eye.z = worldTransform_[0].translation_.z +
			-cos(worldTransform_[0].rotation_.y) * sqrt(viewRadius.x * viewRadius.x + viewRadius.y * viewRadius.y + viewRadius.z * viewRadius.z);

		viewProjection_.target.x = worldTransform_[0].translation_.x;
		viewProjection_.target.y = worldTransform_[0].translation_.y;
		viewProjection_.target.z = worldTransform_[0].translation_.z;

		viewProjection_.UpdateMatrix();
	}

	//弾を撃つ
	{
		for (int i = 4; i < 100; i++) {
			if (bullet[i].isShot == 0) {
				worldTransform_[i].translation_ = { worldTransform_[0].translation_.x,worldTransform_[0].translation_.y,worldTransform_[0].translation_.z };
			}
			else if(bullet[i].isShot == 1) {
				worldTransform_[i].translation_.x += bullet[i].moveX;
				worldTransform_[i].translation_.y += bullet[i].moveY;
				worldTransform_[i].translation_.z += bullet[i].moveZ;
			}
			worldTransform_[i].UpdateMatrix();
		}

		if (input_->PushKey(DIK_SPACE)) {
			for (int i = 4; i < 100; i++) {
				if (bullet[i].isShot == 0) {
					bullet[i].isShot = 1;
					bullet[i].moveX = sin(worldTransform_[0].rotation_.y);
					bullet[i].moveY = 0;
					bullet[i].moveZ = cos(worldTransform_[0].rotation_.y);
					break;
				}
				worldTransform_[i].UpdateMatrix();
			}
		}
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"move:(%f,%f,%f)", worldTransform_[0].translation_.x, worldTransform_[0].translation_.y, worldTransform_[0].translation_.z
	);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z
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
	model_->Draw(worldTransform_[1], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[2], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[3], viewProjection_, textureHandle_);

	for (size_t i = 4; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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