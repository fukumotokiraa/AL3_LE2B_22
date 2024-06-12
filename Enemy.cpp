#include "Enemy.h"
#include "cassert"
#include "Player.h"

void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/mario.jpg");

	worldTransform_.Initialize();
	// 初期座標をセット
	position_ = {0, 10, 50};
	worldTransform_.translation_ = position_;

	velocity_ = {0, 0, EnemySpeed};

	fireTimer_ = kFireInterval;

}

void Enemy::Update() {
	// 座標を移動させる
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
	velocity_ = {0, 0, EnemySpeed};
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);

	for (EnemyBullet* bullet:bullets_) {
		bullet->Update();
	}



	ImGui::Begin(" ");
	ImGui::SliderFloat3("Enemy", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat("EnemyVelocity", &velocity_.z, -1.0f, 1.0f);
	ImGui::End();

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach() {
	EnemySpeed = ApproachSpeed;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	// 発射タイマーをカウントダウン
	fireTimer_--;
	// 指定時間に達した
	if (fireTimer_ == 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Leave() {
	EnemySpeed = LeaveSpeed; 
}

void Enemy::Fire() {
	assert(player_);
	// 玉の速度
	const float kBulletSpeed = 1.5f;
	Vector3 velocity(0, 0, 0);

	player_->GetWorldPosition();
	GetWorldPosition();
	DifferenceVector = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	velocity = Multiply(kBulletSpeed, Normalize(DifferenceVector));

	// 玉を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 玉を登録する
	bullets_.push_back(newBullet);
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 WorldPos; // ワールド座標を入れる変数
	// ワールド行列の平行移動成分を取得(ワールド座標)
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}
