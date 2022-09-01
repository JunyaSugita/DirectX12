#pragma once
#include "MatCalc.h"
#include "Model.h"
#include "ViewProjection.h"
#include <cassert>
#include "Break.h"

class EnemyCore {
  public:
	void Initialize(Vector3 transform, Model* model);
	void Update();
	
	void Draw(ViewProjection viewProjection);
	void OnCollision();

	Vector3 GetWorldPos();
	uint32_t GetHp() { return hp_; }
	bool GetisDead() { return isDead_; }

	void SetHide(bool type) { isHide_ = type; }

  private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	int32_t texture_ = 0;
	int32_t texture2_ = 0;
	int32_t texture3_ = 0;
	int32_t texture4_ = 0;

	uint32_t hp_ = 10;
	bool isDead_ = false;
	bool isHide_ = false;

	//ƒuƒŒƒCƒN
	std::list<std::unique_ptr<Break>> break_;
	bool isBreak = false;
};