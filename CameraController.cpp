#define NOMINMAX
#include "CameraController.h"
#include"Player.h"

float Lerp(float a, float b, float t) { return (1 - t) * a + t * b; }

void CameraController::Initialize(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection; 
	
}

void CameraController::Update() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットと追従対象からカメラの目標座標を計算
	targetCoordinates_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelocity().x * kVelocityBias;
	targetCoordinates_.y = targetWorldTransform.translation_.y + targetOffset_.y + target_->GetVelocity().y * kVelocityBias;
	targetCoordinates_.z = targetWorldTransform.translation_.z + targetOffset_.z + target_->GetVelocity().z * kVelocityBias;
	//座標補間によりゆったり追従
	viewProjection_->translation_.x = Lerp(viewProjection_->translation_.x, targetCoordinates_.x, kInterpolationRate);
	viewProjection_->translation_.y = Lerp(viewProjection_->translation_.y, targetCoordinates_.y, kInterpolationRate);
	viewProjection_->translation_.z = Lerp(viewProjection_->translation_.z, targetCoordinates_.z, kInterpolationRate);
	//追従対象が画面外に出ないように補正
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, target_->GetVelocity().x + margin_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, target_->GetVelocity().x + margin_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, target_->GetVelocity().y + margin_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, target_->GetVelocity().y + margin_.top);
	//移動範囲制限
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);
	//行列を更新する
	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}
