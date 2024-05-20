#define NOMINMAX
#include "Player.h"

float easeInOutCubic(float x) { return x < 0.5 ? 4 * x * x * x : 1 - std::powf(-2 * x + 2, 3) / 2; }
void EaseingX(float& PlayerPosX, const float& StartPosX, const float& EndPosX, float& x) { PlayerPosX = StartPosX + (EndPosX - StartPosX) * easeInOutCubic(x); }

Vector3 Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) { 
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	return Add(center , offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& Position) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	model_ = model;
	worldTransform_.translation_ = Position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	collisionMapInfo.ceiling = false;

	ImGui::Begin(" 適当な名前 ");
	ImGui::Text("velocity.y = %f", velocity_.y);
	ImGui::End();


	Move();



	//移動量に速度の値をコピー
	collisionMapInfo.movement = velocity_;

	//マップ衝突チェック
	MapCollision(collisionMapInfo);

	//ResultMove(collisionMapInfo);
	Ceiling(collisionMapInfo);


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
	//TopCollision(collisionMapInfo);

}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Player::Move() {
	collisionMapInfo.landing = false;
	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 2.0f) {
			collisionMapInfo.landing = true;
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
	// 接地状態
	if (onGround_) {
		//if (isLimitUpper_ == false) {
			if (Input::GetInstance()->PushKey(DIK_UP)) {
				// ジャンプ初速
				velocity_.y += Vector3(0, kJumpAcceleration, 0).y;
				/*if (velocity_.y >= 1) {
					isLimitUpper_ = true;
				}*/
			}
		//}
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
		// 空中
	} else {
		// 落下速度
		velocity_.y += Vector3(0, -kGravityAcceleration, 0).y;
		// 落下制限速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		// 着地
		if (collisionMapInfo.landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向が減衰する
			// velocity_.x *= (1.0f - kAttennuationLanding);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
			//isLimitUpper_ = false;
		}
	}
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::MapCollision(CollisionMapInfo& info) {
	TopCollision(info); 
	//CollisionMapInfo bottomCollision(info);
	//CollisionMapInfo rightCollision(info);
	//CollisionMapInfo leftCollision(info);
}

void Player::TopCollision(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_ , info.movement), static_cast<Corner>(i));
	}
	if (info.movement.y <= 0) {
		return;
	}
	MapChipType mapChipType;
	//真上の当たり判定を行う
	bool hit = false;
	IndexSet indexSet;
	//左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType==MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//ブロックにヒット
	if (hit) {
		//めり込みを排除する方向に移動を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_);
		//めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f) + kBlank);
		//天井に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::ResultMove(CollisionMapInfo& info) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, info.movement); 
}

void Player::Ceiling(CollisionMapInfo& info) {
	if (info.ceiling) {
		velocity_.y = 0;
	}
}
