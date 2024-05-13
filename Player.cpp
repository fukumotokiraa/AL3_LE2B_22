#define NOMINMAX
#include "Player.h"

float easeInOutCubic(float x) { return x < 0.5 ? 4 * x * x * x : 1 - std::powf(-2 * x + 2, 3) / 2; }
void EaseingX(float& PlayerPosX, const float& StartPosX, const float& EndPosX, float& x) { PlayerPosX = StartPosX + (EndPosX - StartPosX) * easeInOutCubic(x); }

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& Position) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	model_ = model;
	worldTransform_.translation_ = Position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	// 移動入力
	//  左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttennuation);
			}
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttennuation);
			}
			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
		}
		// 加速・減速
		velocity_.x += acceleration.x;
		velocity_.y += acceleration.y;
		velocity_.z += acceleration.z;
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 非入力時は移動減衰をかける
		velocity_.x *= (1.0f - kAttennuation);
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		// ジャンプ初速
		velocity_.x += Vector3(0, kJumpAcceleration, 0).x;
		velocity_.y += Vector3(0, kJumpAcceleration, 0).y;
		velocity_.z += Vector3(0, kJumpAcceleration, 0).z;
	}
	// 接地状態
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
		// 空中
	} else {
		// 落下速度
		velocity_.x += Vector3(0, -kGravityAcceleration, 0).x;
		velocity_.y += Vector3(0, -kGravityAcceleration, 0).y;
		velocity_.z += Vector3(0, -kGravityAcceleration, 0).z;
		// 落下制限速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向が減衰する
			// velocity_.x *= (1.0f - kAttennuationLanding);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	// 行列計算
	worldTransform_.UpdateMatrix();
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= (1.0f / 60.0f);
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		float t = 1.0f - (turnTimer_ / kTimeTurn);
		float easeInOut{};
		EaseingX(easeInOut, turnFirstRotationY_, destinationRotationY, t);
		worldTransform_.rotation_.y = easeInOut;
	}
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
