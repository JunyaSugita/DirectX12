#include "Enemy.h"

void (Enemy::*Enemy::phaseFuncTable[])() = {
  &Enemy::ApproachFunc,
  &Enemy::LeaveFunc,
};

void Enemy::Initialize(Model* model, WorldTransform playerTransform) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy.png");
	//���f���ǂݍ���
	bulletModel_ = Model::CreateFromOBJ("enemyBullet", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {20.0f, 30.0f, 150.0f};

	bulletCenter_.Initialize();

	playerTransform_ = playerTransform;

	ApproachInitialize();
}

void Enemy::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();
	MatCalc(worldTransform_);

	bulletCenter_.translation_ = playerTransform_.translation_;
	bulletCenter_.rotation_ = playerTransform_.rotation_;
	bulletCenter_.translation_ += {cos(Radian(playerAngle_ - 90)) * -60, 8, sin(Radian(playerAngle_ - 90)) * -60};
	MatCalc(bulletCenter_);

	playerPosition = playerTransform_.translation_;

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		//�e�̑��x
		const float kBulletSpeed = 0.15f;

		Vector3 playerLen = playerPosition - bullet->GetWorldPos();
		playerLen.normalize();
		playerLen *= kBulletSpeed;

		bullet->SetVelocity(playerLen);
		bullet->SetWorldTransform(bulletCenter_);
		bullet->SetPlayerAngle(playerAngle_);

		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachFunc() {
	coolTime--;
	if (hp == 4 && coolTime == 0) {
		Fire(Vector3(0, 0, 0),0);
		Fire(Vector3(2, 0, 0),0);
		Fire(Vector3(-2, 0, 0), 0);
	}
	if (hp == 3 && coolTime == 0) {
		Fire(Vector3(-2, 0, 0), 0);
		Fire(Vector3(6, 0, 0), 1);
		Fire(Vector3(2, 0, 0), 0);
		Fire(Vector3(-6, 0, 0), 1);
	}
	if (hp == 2 && coolTime == 0) {
		Fire(Vector3(-3, -1, 0), 1);
		Fire(Vector3(3, -1, 0), 1);
		Fire(Vector3(0, 1, 0), 0);
		Fire(Vector3(3, 1, 0), 0);
		Fire(Vector3(0, -1, 0), 1);
		Fire(Vector3(-3, 1, 0), 0);
	}
	if (hp == 1 && coolTime == 0) {
		Fire(Vector3(-6, 1, 0), 0);
		Fire(Vector3(-3, 1, 0), 0);
		Fire(Vector3(0, 1, 0), 0);
		Fire(Vector3(3, 1, 0), 0);
		Fire(Vector3(6, 1, 0), 0);
		Fire(Vector3(-6, -1, 0), 1);
		Fire(Vector3(-3, -1, 0), 1);
		Fire(Vector3(0, -1, 0), 1);
		Fire(Vector3(3, -1, 0), 1);
		Fire(Vector3(6, -1, 0), 1);
	}

	//����̈ʒu�܂ŗ����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveFunc() {
	//�ړ�
	// worldTransform_.translation_.z += 0.5f;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire(Vector3 transform,int type) {
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_,bulletModel_,bulletCenter_, transform,type);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize() {
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

int Enemy::GetCoolTime() { return coolTime; }

void Enemy::SetWorldTransform(WorldTransform playerTransform) {
	playerTransform_ = playerTransform;
}

void Enemy::SetPlayerAngle(float angle) { playerAngle_ = angle; }

void Enemy::SetInput(int num) { hp += num; }

void Enemy::SetCoolTime(int num) { coolTime = num; }

void Enemy::OnCollision() {}

float Enemy::Radian(float r) { return r * (3.14159265f / 180); }