#include "Enemy.h"
#include "cassert"

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
}

void Enemy::Update() {
	// 座標を移動させる
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);

	ImGui::Begin(" ");
	ImGui::SliderFloat3("Enemy", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat("EnemyVelocity", &velocity_.z, -1.0f, 1.0f);
	ImGui::End();

	// 行列を更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
