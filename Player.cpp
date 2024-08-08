#include "Player.h"
#include<cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, ViewProjection* viewProjection) { 
	worldTransform_.Initialize();
	assert(modelBody);
	worldTransformBody_.Initialize();
	modelBody_ = modelBody;
	textureBody_ = TextureManager::Load("./Resources/float_Body/tex.png");

	assert(modelHead);
	worldTransformHead_.Initialize();
	modelHead_ = modelHead;
	textureHead_ = TextureManager::Load("./Resources/float_Head/tex.png");

	assert(modelL_arm);
	worldTransformL_arm_.Initialize();
	modelL_arm_ = modelL_arm;
	textureL_arm_ = TextureManager::Load("./Resources/float_L_arm/tex.png");

	assert(modelR_arm);
	worldTransformR_arm_.Initialize();
	modelR_arm_ = modelR_arm;
	textureR_arm_ = TextureManager::Load("./Resources/float_R_arm/tex.png");

	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformR_arm_.translation_ = {0.6f, 1.2f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.6f, 1.2f, 0.0f};

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	InitializeFloatingGimmick();

	//worldTransform_.Initialize();
	//model_ = model;
	//textureHandle_ = TextureManager::Load("./Resources/Player/tex.png");
	viewProjection_ = viewProjection;

}

void Player::Update() { 

	XINPUT_STATE joyState;

	UpdateFloatingGimmick();

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


	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	//	// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix(); 
	//worldTransformBody_.TransferMatrix();
	//worldTransformHead_.TransferMatrix();
	//worldTransformL_arm_.TransferMatrix();
	//worldTransformR_arm_.TransferMatrix();
}

void Player::Draw() { 
	//model_->Draw(worldTransform_, *viewProjection_);
	modelBody_->Draw(worldTransformBody_, *viewProjection_);
	modelHead_->Draw(worldTransformHead_, *viewProjection_);
	modelL_arm_->Draw(worldTransformL_arm_, *viewProjection_);
	modelR_arm_->Draw(worldTransformR_arm_, *viewProjection_);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	//浮遊のサイクル
	int period = 90; 
	const float step = 2.0f * (float)M_PI / period;

	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * (float)M_PI);
	//浮遊の振幅
	float floatAmplitude = 0.1f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatAmplitude;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * floatAmplitude;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * floatAmplitude;


	ImGui::Begin(" ");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Body", &worldTransformBody_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("L_arm", &worldTransformL_arm_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("R_arm", &worldTransformR_arm_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderInt("Period", &period, -100, 100);
	ImGui::SliderFloat("FloatAmplitude", &floatAmplitude, -50.0f, 50.0f);
	ImGui::End();
}
