#include "EnemyCore.h"

void EnemyCore::Initialize(Vector3 transform, Model* model) {
	assert(model);

	model_ = model;
	texture_ = TextureManager::Load("enemyCore.png");
	texture2_ = TextureManager::Load("enemyCore2.png");
	texture3_ = TextureManager::Load("enemyCore3.png");
	texture4_ = TextureManager::Load("enemyCore4.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = transform;
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	MatCalc(worldTransform_);
}

void EnemyCore::Update() {
	//デスフラグの立った弾を削除
	break_.remove_if([](std::unique_ptr<Break>& bullet) { return bullet->IsDead(); });

	if (isHide_ == true && isBreak == false) {
		isBreak = true;
		// Break
		std::unique_ptr<Break> newBreak = std::make_unique<Break>();
		worldTransform_.translation_.y += 10.0f;
		newBreak->Inisialize(model_, texture_, worldTransform_);

		break_.push_back(std::move(newBreak));
	}

	for (std::unique_ptr<Break>& bubble : break_) {
		bubble->Update();
	}
}

void EnemyCore::Draw(ViewProjection viewProjection) {
	if (isHide_ == false) {
		if (hp_ == 10) {
			model_->Draw(worldTransform_, viewProjection, texture_);
		} else if (hp_ >= 7) {
			model_->Draw(worldTransform_, viewProjection, texture2_);
		} else if (hp_ >= 4) {
			model_->Draw(worldTransform_, viewProjection, texture3_);
		} else {
			model_->Draw(worldTransform_, viewProjection, texture4_);
		}
	}
	for (std::unique_ptr<Break>& break1 : break_) {
		break1->Draw(viewProjection);
	}
}

void EnemyCore::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

Vector3 EnemyCore::GetWorldPos() {
	Vector3 a = worldTransform_.translation_;
	return a;
}