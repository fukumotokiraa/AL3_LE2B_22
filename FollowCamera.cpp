#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();

}

void FollowCamera::Update() {
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float RotateSpeed = 0.000001f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX * RotateSpeed;
	}
	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		// カメラの角度から回転行列を計算する
		rotationMatrix = MakeRotateMatrix(viewProjection_.rotation_);
		// オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotationMatrix);
		viewProjection_.translation_ = target_->translation_ + offset;
	}


	viewProjection_.UpdateMatrix();
	//viewProjection_.TransferMatrix();
	
}
