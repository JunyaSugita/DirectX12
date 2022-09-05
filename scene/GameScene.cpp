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
	delete modelSkydome_;
	delete enemy_;
	delete skydome_;
	delete sprite_;
	delete modelEnemy_;
	delete modelEnemyBullet_;
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
	textureHandle_ = TextureManager::Load("player.png");
	scopeHandle_ = TextureManager::Load("red10x10.png");
	waterHandle_ = TextureManager::Load("water.png");
	lifeHandle_ = TextureManager::Load("Life.png");
	titleHandle_ = TextureManager::Load("title.png");
	startHandle_ = TextureManager::Load("start.png");
	goTitleHandle_ = TextureManager::Load("goTitle.png");
	goRetryHandle_ = TextureManager::Load("goRetry.png");
	gameClearHandle_ = TextureManager::Load("gameClear.png");
	gameoverHandle_ = TextureManager::Load("gameover.png");
	setumeiHandle_ = TextureManager::Load("setumei1.png");
	setumei2Handle_ = TextureManager::Load("setumei2.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelEnemy_ = Model::CreateFromOBJ("ere", true);
	modelEnemyBullet_ = Model::CreateFromOBJ("enemyBullet", true);
	//ビュープロジェクションの初期化
	for (int i = 0; i < 5; i++) {
		viewProjection_[i].Initialize();
		viewProjection_[i].farZ = 12000.0f;
	}

	viewProjection_[0].eye = {0.0f, 10.0f, -100.0f};

	viewProjection_[1].eye = {-80, 135, -150};
	viewProjection_[1].target = {-30, 85, -100};

	viewProjection_[2].eye = {-80, 145, 90};
	viewProjection_[2].target = {-30, 95, 40};

	viewProjection_[3].eye = {80, 135, -150};
	viewProjection_[3].target = {30, 85, -100};

	viewProjection_[4].eye = {80, 145, 90};
	viewProjection_[4].target = {30, 95, 40};

	for (int i = 0; i < 5; i++) {
		viewProjection_[i].UpdateMatrix();
	}

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	

	//軸方向表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラ生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(textureHandle_);

	//敵キャラ生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_,modelEnemy_, player_->GetWorldTransform(),player_->GetFrontVec());
	enemy_->SetPlayer(player_);

	//天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//水
	water_.Initialize();
	water_.translation_ = {0.0f, -1.0f, 0.0f};
	water_.scale_ = {1000.0f, 1.0f, 1000.0f};
	MatCalc(water_);

	//カメラ関係
	cameraFrontVec = viewProjection_[0].target - viewProjection_[0].eye;

	//スプライト
	sprite_ = Sprite::Create(scopeHandle_, {635, 355});
	lifeSprite_[0] = Sprite::Create(lifeHandle_, {0, 0});
	lifeSprite_[1] = Sprite::Create(lifeHandle_, {100, 0});
	lifeSprite_[2] = Sprite::Create(lifeHandle_, {200, 0});
	titleSprite_ = Sprite::Create(titleHandle_, {0, 0});
	startSprite_ = Sprite::Create(startHandle_, {0, 0});
	goTitleSprite_ = Sprite::Create(goTitleHandle_, {0, 0});
	goRetrySprite_ = Sprite::Create(goRetryHandle_, {0, 0});
	gameClearSprite_ = Sprite::Create(gameClearHandle_, {0, 0});
	gameoverSprite_ = Sprite::Create(gameoverHandle_, {0, 0});
	setumeiSprite_ = Sprite::Create(setumeiHandle_, {0, 0});
	setumei2Sprite_ = Sprite::Create(setumei2Handle_, {0, 0});

	//サウンド
	gameBGM_ = audio_->LoadWave("sound/gameBGM.wav");
	audio_->PlayWave(gameBGM_, true, 0.1f);

	iceBreakSE_ = audio_->LoadWave("sound/iceBreak.wav");
}

void GameScene::Update() {
#ifdef _DEBUG
	isDebugCameraActive_ = false;
	if (input_->TriggerKey(DIK_P)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif

	if (scene == title) {
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = main;
			///初期化
			delete player_;
			delete enemy_;
			//自キャラ生成
			player_ = new Player();
			//自キャラの初期化
			player_->Initialize(textureHandle_);
			//敵キャラ生成
			enemy_ = new Enemy();
			//敵キャラの初期化
			enemy_->Initialize(model_, modelEnemy_, player_->GetWorldTransform(), player_->GetFrontVec());
			enemy_->SetPlayer(player_);
			deathTimer = deathTime;
			moveCoolTimer = moveCoolTime;
		}
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_[0] = debugCamera_->GetViewProjection();
			viewProjection_[0].TransferMatrix();
		} else {
			viewProjection_[0].TransferMatrix();
		}

		int cameraLen = 10;
		viewProjection_[0].target = player_->GetWorldPosition();
		viewProjection_[0].target.y += 3.0f;

		viewProjection_[0].eye = viewProjection_[0].target;
		viewProjection_[0].eye +=
		  {cos(Radian(cameraAngleX)) * cameraLen * cos(Radian(cameraAngleY)),
		   sin(Radian(cameraAngleY)) * cameraLen,
		   sin(Radian(cameraAngleX)) * cameraLen * cos(Radian(cameraAngleY))};
		if (viewProjection_[0].eye.y < 0.0f) {
			viewProjection_[0].eye.y = 0.0f;
		}
		viewProjection_[0].UpdateMatrix();

		//自キャラ更新
		player_->SetCameraVec(viewProjection_[0].target - viewProjection_[0].eye);
		player_->Update();
		enemy_->SetWorldTransform(player_->GetWorldTransform());
	}
	if (scene == gameCrear) {
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = title;
		}
	}
	if (scene == gameover) {
		player_->naosu();
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = title;
		}
		if (input_->TriggerKey(DIK_R)) {
			scene = main;
			///初期化
			delete player_;
			delete enemy_;
			//自キャラ生成
			player_ = new Player();
			//自キャラの初期化
			player_->Initialize(textureHandle_);
			//敵キャラ生成
			enemy_ = new Enemy();
			//敵キャラの初期化
			enemy_->Initialize(
			  model_, modelEnemy_, player_->GetWorldTransform(), player_->GetFrontVec());
			enemy_->SetPlayer(player_);
			deathTimer = deathTime;
			moveCoolTimer = moveCoolTime;
		}
	}
	if (scene == main) {

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_[0] = debugCamera_->GetViewProjection();
			viewProjection_[0].TransferMatrix();
		} else {
			viewProjection_[0].TransferMatrix();
		}

		if (deathTimer == deathTime || deathTimer % 80 == 0) {

			int cameraLen = 10;
			viewProjection_[0].target = player_->GetWorldPosition();
			viewProjection_[0].target.y += 3.0f;

			viewProjection_[0].eye = viewProjection_[0].target;
			viewProjection_[0].eye +=
			  {cos(Radian(cameraAngleX)) * cameraLen * cos(Radian(cameraAngleY)),
			   sin(Radian(cameraAngleY)) * cameraLen,
			   sin(Radian(cameraAngleX)) * cameraLen * cos(Radian(cameraAngleY))};
			if (viewProjection_[0].eye.y < 0.0f) {
				viewProjection_[0].eye.y = 0.0f;
			}
			viewProjection_[0].UpdateMatrix();

			//自キャラ更新
			player_->SetCameraVec(viewProjection_[0].target - viewProjection_[0].eye);
			player_->Update();
			enemy_->SetWorldTransform(player_->GetWorldTransform());

			//敵更新
			if (enemy_ != NULL) {
				enemy_->Update();
			}

			CheckAllCollision();

			skydome_->Update();

			enemy_->SetPlayerAngle(player_->GetPlayerAngle());

			/// <summary>
			/// レイの判定
			/// </summary>
			cameraFrontVec = viewProjection_[0].target - viewProjection_[0].eye;
			cameraFrontVec.normalize();

			//敵弾リストの取得
			const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
			int Count = 0;
			bool isMove = false;
			cameraSpeed = 0.3f;
			moveCoolTimer--;
			for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
				Count++;
				if (bullet->GetisMove() == true) {
					isMove = true;
				}

				if (bullet->GetisMove() == false && isMove == false && moveCoolTimer <= 0) {
					bullet->Move();
					isMove = true;
				}

				bullet->SetBulletVec(bullet->GetWorldPos() - viewProjection_[0].eye);
				bullet->BulletVecNormalize();
				Vector3 bulletPos = bullet->GetWorldPos();
				float len;
				len = sqrt(
				  (bulletPos.x - viewProjection_[0].eye.x) *
				    (bulletPos.x - viewProjection_[0].eye.x) +
				  (bulletPos.z - viewProjection_[0].eye.z) *
				    (bulletPos.z - viewProjection_[0].eye.z));
				cameraFrontVec *= len;
				bullet->SetBulletVec(bullet->GetBulletVec() * len);
				Vector3 hitCheck_;
				hitCheck_ = cameraFrontVec - bullet->GetBulletVec();
				if (
				  hitCheck_.x <= 1.0f && hitCheck_.x >= -1.0f && hitCheck_.y <= 1.0f &&
				  hitCheck_.y >= -1.0f && hitCheck_.z <= 1.0f && hitCheck_.z >= -1.0f) {
					debugText_->SetPos(0, 20);
					debugText_->Printf("Hit!");
					if (
					  hitCheck_.x <= 0.5f && hitCheck_.x >= -0.5f && hitCheck_.y <= 0.5f &&
					  hitCheck_.y >= -0.5f && hitCheck_.z <= 0.5f && hitCheck_.z >= -0.5f &&
					  bullet->GetisHit() == false) {
						cameraSpeed = 0.1f;
					}
					if (input_->TriggerKey(DIK_SPACE)) {
						bullet->SetisHit(true);
						moveCoolTimer = moveCoolTime;
						audio_->PlayWave(iceBreakSE_, false, 0.1f);
					}
				}
			}
			if (isMove == false && enemy_->GetCoolTime() < 0 && moveCoolTimer < 0) {
				player_->SetType(1);
			}

			if (input_->PushKey(DIK_RIGHT)) {
				cameraAngleX -= cameraSpeed;
				if (cameraAngleX < 0.0f) {
					cameraAngleX += 360.0f;
				}
			}
			if (input_->PushKey(DIK_LEFT)) {
				cameraAngleX += cameraSpeed;
				if (cameraAngleX > 360.0f) {
					cameraAngleX -= 360.0f;
				}
			}
			if (input_->PushKey(DIK_UP)) {
				cameraAngleY -= cameraSpeed;
				if (cameraAngleY < -89.5f) {
					cameraAngleY = -89.5f;
				}
			}
			if (input_->PushKey(DIK_DOWN)) {
				cameraAngleY += cameraSpeed;
				if (cameraAngleY > 89.5f) {
					cameraAngleY = 89.5f;
				}
			}
		}
		if (player_->GetLife() <= 0) {
			deathTimer--;
			player_->taoreru();
			if (deathTimer <= 0) {
				scene = gameover;
			}
		}
		if (enemy_->GetLife() <= 0 && cameraNum_ == 0) {
			scene = gameCrear;
		}

		if (cameraNum_ != 0) {
			cameraTimer--;
			if (cameraTimer == 200) {
				enemy_->core_[cameraNum_ - 1]->SetHide(true);
				audio_->PlayWave(iceBreakSE_, false, 0.1f);
			}
			if (cameraTimer <= 0) {
				cameraNum_ = 0;
				cameraTimer = cameraTime;
			}
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
	
	if (scene == title) {
		player_->Draw(viewProjection_[cameraNum_]);
	}
	if (scene == main) {
		//自キャラの更新
		player_->Draw(viewProjection_[cameraNum_]);

		//敵
		if (enemy_ != NULL) {
			enemy_->Draw(viewProjection_[cameraNum_]);
		}

		//天球
		skydome_->Draw(viewProjection_[cameraNum_]);

		//水
		model_->Draw(water_, viewProjection_[cameraNum_], waterHandle_);
	}
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	/*for (int i = 0; i < 21; i++) {
	    PrimitiveDrawer::GetInstance()->DrawLine3d(
	      Vector3(100, -2, i * 10 - 100), Vector3(-100, -2, i * 10 - 100), Vector4(255, 0, 0, 255));
	    PrimitiveDrawer::GetInstance()->DrawLine3d(
	      Vector3(i * 10 - 100, -2, 100), Vector3(i * 10 - 100, -2, -100), Vector4(0, 0, 255, 255));
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	if (scene == title) {
		titleSprite_->Draw();
		startSprite_->Draw();
	}
	if (scene == main) {
		if (cameraNum_ == 0) {
			sprite_->Draw();
		}
		if (player_->GetLife() >= 1) {
			lifeSprite_[0]->Draw();
		}
		if (player_->GetLife() >= 2) {
			lifeSprite_[1]->Draw();
		}
		if (player_->GetLife() >= 3) {
			lifeSprite_[2]->Draw();
		}

		if (player_->GetType() == 0) {
			setumeiSprite_->Draw();
		}
		if (player_->GetType() == 1) {
			setumei2Sprite_->Draw();
		}
	}
	if (scene == gameCrear) {
		gameClearSprite_->Draw();
		goTitleSprite_->Draw();
	}
	if (scene == gameover) {
		gameoverSprite_->Draw();
		goRetrySprite_->Draw();
	}
	// デバッグテキストの描画

	// debugText_->DrawAll(commandList);
	//
	//  スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

float GameScene::Radian(float r) { return r * (PI / 180); }

void GameScene::CheckAllCollision() {
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPos();

		Vector3 len;
		len = posB - posA;

		if (len.length() <= 2) {
			if (bullet->GetisHit() == false) {
				player_->OnCollision();
				bullet->OnCollision();
				audio_->PlayWave(iceBreakSE_, false, 0.1f);
			}
			moveCoolTimer = moveCoolTime;
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//敵キャラと自分の弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPos();

		Vector3 len;
		len = posB - posA;

		if (len.length() <= 2) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵のコアの当たり判定
	for (int i = 0; i < 4;i++) {
		//敵コアの座標
		if (enemy_->core_[i]->GetisDead() == false) {
			posA = enemy_->core_[i]->GetWorldPos();
			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
				posB = bullet->GetWorldPos();

				Vector3 len;
				len = posA - posB;

				if (len.length() <= 5) {
					enemy_->core_[i]->OnCollision();
					bullet->OnCollision();
					audio_->PlayWave(iceBreakSE_, false, 0.1f);
					if (enemy_->core_[i]->GetHp() <= 0) {
						player_->SetType(0);
						moveCoolTimer = moveCoolTime;
						enemy_->SetInput(-1);
						enemy_->SetCoolTime(30 * 60);
						cameraNum_ = i + 1;
					}
				}
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (const std::unique_ptr<EnemyBullet>& eneBullet : enemyBullets) {
		//敵キャラの座標
		posA = eneBullet->GetWorldPos();

		//敵キャラと自分の弾全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
			//敵弾の座標
			posB = playerBullet->GetWorldPos();

			Vector3 len;
			len = posB - posA;

			if (len.length() <= 2) {
				eneBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
