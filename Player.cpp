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

	if(input_->)
}

void Player::Draw(){}