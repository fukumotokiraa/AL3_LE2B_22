#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/Player/tex.png");
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
}

void Player::Update() { 

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed
		};
		//移動量に速さを反映
		float length = sqrtf(move.x * move.x + move.y * move.y + move.z * move.z);
		if (length > 0.0f) {
			move = Normalize(move) * speed;
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);
			
		} else {
			move = {0.0f, 0.0f, 0.0f};
		}
		//move = TransformNormal(move, followCamera_->GetRotateMatrix());

		        // XZ平面上の移動ベクトルを作成（Y成分を0にする）
		Vector3 moveXZ = {move.x, 0.0f, move.z};
		// XZ平面上のベクトルの長さを計算
		float lengthXZ = sqrtf(moveXZ.x * moveXZ.x + moveXZ.z * moveXZ.z);

		// 移動方向に応じてキャラクターの向きを設定
		if (lengthXZ > 0.0f) {
			// Y軸周りの回転角度を計算
			worldTransform_.rotation_.y = std::atan2(moveXZ.x, moveXZ.z);
			// X軸周りの回転角度を計算
			worldTransform_.rotation_.x = std::atan2(move.y, lengthXZ);
		}
		//移動
		worldTransform_.translation_ = worldTransform_.translation_ + move;

	}
	ImGui::Begin(" ");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::End();

	worldTransform_.UpdateMatrix();
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }
