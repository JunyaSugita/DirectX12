#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>

class EnemyCore {
  public:
	void Initialize(Vector3 transform, Model* model);
	void Update();
	
	void Draw(ViewProjection viewProjection);

  private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	int32_t textur_ = 0;
};