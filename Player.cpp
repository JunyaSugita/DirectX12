#include "Player.h"
#include "assert.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	MatCalc(worldTransform_);
}

void Player::Update(){
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_W)) {
		move.z += 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		move.z -= 1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		move.x -= 1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += 1.0f;
	}
	MatCalc(worldTransform_);

	debugText_->SetPos(0, 0);
	debugText_->Printf("player = %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Player::Draw(){}