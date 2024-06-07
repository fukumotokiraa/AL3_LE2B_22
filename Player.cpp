#include "Player.h"
#include <cassert>

Vector3 Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	textureHandle_ = textureHandle;
	model_ = model;
	input_ = Input::GetInstance();
}

void Player::Update() {
	//// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix();



	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	ImGui::Begin(" ");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::End();

	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }