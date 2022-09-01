#pragma once
#include "WorldTransform.h"
#include "model.h"
#include <cassert>
#include "MatCalc.h"

class Skydome {
  public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

  private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t texture_ = 0;
};
