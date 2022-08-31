#include "Bubble.h"
#include <cassert>
#include <random>

void Bubble::Inisialize(Model* model, uint32_t textur, WorldTransform playerTransform) {
	assert(model);

	//ƒ‰ƒ“ƒ_ƒ€
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> x(-0.05f, -0.01f);
	std::uniform_real_distribution<float> y(0.00f, 0.01f);
	std::uniform_real_distribution<float> z(-0.01f, 0.01f);

	model_ = model;
	textur_ = textur;

	worldTransform_.Initialize();
	worldTransform_.parent_ = &playerTransform;
	worldTransform_.scale_ = {0.01f, 0.01f, 0.01f};
	worldTransform_.translation_.x = 0.16f;
	MatCalc(worldTransform_);

	move_ = {x(engine), y(engine), z(engine)};
}

void Bubble::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += move_;
	move_.y -= 0.001f;
	if (worldTransform_.translation_.y < 0) {
		isDead_ = true;
	}
	MatCalc(worldTransform_);
}

void Bubble::Move() {}

void Bubble::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textur_);
}