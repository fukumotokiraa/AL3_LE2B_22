#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/Player/tex.png");
}

void Player::Update() { 

	//XINPUT_STATE joyState;

	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	//速さ
	//	const float speed = 0.3f;
	//	//移動量
	//	Vector3 move = {
	//		(float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed,
	//		0.0f,
	//		(float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed
	//	};
	//	//移動量に速さを反映
	//	move = Normalize(move) * speed;
	//	//移動
	//	worldTransform_.translation_ = worldTransform_.translation_ + move;

	//}
	//ImGui::Begin(" ");
	//ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -50.0f, 50.0f);
	//ImGui::End();

	//worldTransform_.UpdateMatrix();
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }
