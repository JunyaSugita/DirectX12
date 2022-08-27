#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Bubble{
  public:
	void Inisialize(Model* model, uint32_t textur, WorldTransform playerTransform);
	void Update();
	void Move();

	void Draw(ViewProjection viewProjection);

	bool IsDead() const { return isDead_; }
  private:
	WorldTransform worldTransform_;

	Vector3 move_;

	Model* model_ = nullptr;
	uint32_t textur_ = 0;

	static const int32_t kLifeTime = 60;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};