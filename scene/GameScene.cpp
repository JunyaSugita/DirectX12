#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "random"

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

	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//x,y,z軸周りの平行移動を設定
		worldTransform.translation_ = { dist(engine),dist(engine),dist(engine)};

		//平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matTrans.m[i][j] = 1;
				}
				else {
					matTrans.m[i][j] = 0;
				}
			}
		}

		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;


		worldTransform.rotation_ = { rot(engine),rot(engine),rot(engine) };
		//Z軸回転
		Matrix4 matRotZ;
		//設定
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRotZ.m[i][j] = 1;
				}
				else {
					matRotZ.m[i][j] = 0;
				}
			}
		}
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);

		//X軸回転
		Matrix4 matRotX;
		//設定
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRotX.m[i][j] = 1;
				}
				else {
					matRotX.m[i][j] = 0;
				}
			}
		}
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);

		//Y軸回転
		Matrix4 matRotY;
		//設定
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					matRotY.m[i][j] = 1;
				}
				else {
					matRotY.m[i][j] = 0;
				}
			}
		}
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);

		Matrix4 matRot;

		matRot = matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;

		//単位行列を代入

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					worldTransform.matWorld_.m[i][j] = 1;
				}
				else {
					worldTransform.matWorld_.m[i][j] = 0;
				}
			}
		}

		worldTransform.matWorld_ *= matTrans;
		worldTransform.matWorld_ *= matRot;

		//行列の転送
		worldTransform.TransferMatrix();
	}

	//視野角
	viewProjection_.fovAngleY = Radian(10.0f);


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

	//視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = { 0,0,0 };

		//移動速度
		const float kEyeSpeed = 0.2f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}

		//視点移動
		viewProjection_.eye += move;

		
	}

	//fov
	{
		//視野変更
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += Radian(1.0f);
			if (viewProjection_.fovAngleY >= Radian(180.0f)) {
				viewProjection_.fovAngleY = Radian(180.0f);
			}
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= Radian(1.0f);
			if (viewProjection_.fovAngleY <= Radian(0.0f)) {
				viewProjection_.fovAngleY = Radian(0.0f);
			}
		}
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();
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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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