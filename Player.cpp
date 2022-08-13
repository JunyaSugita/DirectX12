#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet> & bullet) { 
		return bullet->IsDead(); 
	});

	//移動
	Vector3 move = {0, 0, 0};

	//移動速度
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move += {-kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_D)) {
		move += {kCharacterSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_W)) {
		move += {0, kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_S)) {
		move += {0, -kCharacterSpeed, 0};
	}
	worldTransform_.translation_ += move;

	//移動限界
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX); 
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX); 
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY); 

	//旋回
	Vector3 rotate = {0, 0, 0};

	//移動速度
	const float krotateSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		rotate += {0, -krotateSpeed, 0};
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotate += {0, krotateSpeed, 0};
	}
	if (input_->PushKey(DIK_UP)) {
		rotate += {-krotateSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_DOWN)) {
		rotate += {krotateSpeed, 0, 0};
	}
	worldTransform_.rotation_ += rotate;

	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf(
		"%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);

	//キャラクターの攻撃
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();	
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection); 	
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機の向きに合わせる
		velocity *= worldTransform_.matWorld_;

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos; 
}

void Player::OnCollision() {
	
}
