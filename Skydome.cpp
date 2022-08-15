#include "Skydome.h"

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
	MatCalc(worldTransform_);
}

void Skydome::Update() {}

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
