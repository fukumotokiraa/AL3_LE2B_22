#include "Player.h"
#include<cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	assert(models.size() == 4); // 4つのモデルが渡されることを確認
	worldTransform_.Initialize();

	// ボディ
	assert(models[0]);
	worldTransformBody_.Initialize();
	modelBody_ = models[0];
	textureBody_ = TextureManager::Load("./Resources/float_Body/tex.png");

	// ヘッド
	assert(models[1]);
	worldTransformHead_.Initialize();
	modelHead_ = models[1];
	textureHead_ = TextureManager::Load("./Resources/float_Head/tex.png");

	// 左腕
	assert(models[2]);
	worldTransformL_arm_.Initialize();
	modelL_arm_ = models[2];
	textureL_arm_ = TextureManager::Load("./Resources/float_L_arm/tex.png");

	// 右腕
	assert(models[3]);
	worldTransformR_arm_.Initialize();
	modelR_arm_ = models[3];
	textureR_arm_ = TextureManager::Load("./Resources/float_R_arm/tex.png");

	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformR_arm_.translation_ = {0.6f, 1.2f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.6f, 1.2f, 0.0f};

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	InitializeFloatingGimmick();
}


void Player::Update() { 
	XINPUT_STATE joyState;

	BaseCharacter::Update();

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

void Player::Draw(ViewProjection* viewProjection) { 
	models_[kModelIndexBody]->Draw(worldTransformBody_, *viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, *viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, *viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, *viewProjection);

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
