#include "Break.h"
#include <random>
#include <cassert>

void Break::Inisialize(Model* model, uint32_t textur, WorldTransform bulletTrans) {
	assert(model);

	//ƒ‰ƒ“ƒ_ƒ€
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	for (int i = 0; i < 8; i++) {
		std::uniform_real_distribution<float> x(-0.01f, 0.01f);
		std::uniform_real_distribution<float> y(0.01f, 0.1f);
		std::uniform_real_distribution<float> z(-0.01f, 0.01f);

		std::uniform_real_distribution<float> rx(-0.05f, 0.05f);
		std::uniform_real_distribution<float> ry(-0.05f, 0.05f);
		std::uniform_real_distribution<float> rz(-0.05f, 0.05f);

		move_[i] = {x(engine), y(engine), z(engine)};
		rotate_[i] = {rx(engine), ry(engine), rz(engine)};
	}

	model_ = model;
	textur_ = textur;
	for (int i = 0; i < 8; i++) {
		worldTransform_[i].Initialize();
		worldTransform_[i].translation_ = bulletTrans.translation_;
		worldTransform_[i].scale_ = {0.5f, 0.5f, 0.5f};
	}
	worldTransform_[0].translation_ += {0.25f, 0.25f, 0.25f};
	worldTransform_[1].translation_ += {-0.25f, 0.25f, 0.25f};
	worldTransform_[2].translation_ += {0.25f, -0.25f, 0.25f};
	worldTransform_[3].translation_ += {-0.25f, -0.25f, 0.25f};

	worldTransform_[4].translation_ += {0.25f, 0.25f, -0.25f};
	worldTransform_[5].translation_ += {-0.25f, 0.25f, -0.25f};
	worldTransform_[6].translation_ += {0.25f, -0.25f, -0.25f};
	worldTransform_[7].translation_ += {-0.25f, -0.25f, -0.25f};

	for (int i = 0; i < 8; i++) {
		MatCalc(worldTransform_[i]);
	}
	
}

void Break::Update() {
	for (int i = 0; i < 8; i++) {
		worldTransform_[i].translation_ += move_[i];
		move_[i].y -= 0.001f;
		if (worldTransform_[i].translation_.y < 0) {
			isDead_ = true;
		}
		worldTransform_[i].rotation_ += rotate_[i];
		MatCalc(worldTransform_[i]);
	}
}

void Break::Move() {}

void Break::Draw(ViewProjection viewProjection) {
	for (int i = 0; i < 8; i++) {
		model_->Draw(worldTransform_[i], viewProjection, textur_);
	}
}