#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models); 

	assert(models.size() == 1); 
	worldTransform_.Initialize();

	assert(models[0]);
	worldTransformBody_.Initialize();
	modelBody_ = models[0];
	textureBody_ = TextureManager::Load("./Resources/needle_Body/e_head.png");

	worldTransformBody_.parent_ = &worldTransform_;
}

void Enemy::Update() { 
	BaseCharacter::Update(); 

	const float speed = 0.01f;
	// 時間経過を利用して角度を計算
	angle += speed; // 回転速度を調整

	// Y軸を中心とした円軌道の計算
	float radius = 5.0f; // 円軌道の半径を設定
	Vector3 move = {
	    std::cos(angle) * radius, // X軸方向の移動
	    0.0f,                     // Y軸方向の移動はなし
	    std::sin(angle) * radius  // Z軸方向の移動
	};
	Vector3 forward = {
	    -std::sin(angle), // X軸方向
	    0.0f,             // Y軸方向
	    std::cos(angle)   // Z軸方向
	};
	//// 移動量に速さを反映
	//float length = sqrtf(move.x * move.x + move.y * move.y + move.z * move.z);
	//if (length > 0.0f) {
	//	move = Normalize(move) * speed;
	//	//Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
	//	//move = TransformNormal(move, rotateMatrix);

	//} else {
	//	move = {0.0f, 0.0f, 0.0f};
	//}
	//// XZ平面上の移動ベクトルを作成（Y成分を0にする）
	//Vector3 moveXZ = {move.x, 0.0f, move.z};
	//// XZ平面上のベクトルの長さを計算
	//float lengthXZ = sqrtf(moveXZ.x * moveXZ.x + moveXZ.z * moveXZ.z);

	//// 移動方向に応じてキャラクターの向きを設定
	//if (lengthXZ > 0.0f) {
	//	// Y軸周りの回転角度を計算
	//	worldTransform_.rotation_.y = std::atan2(moveXZ.x, moveXZ.z);
	//	// X軸周りの回転角度を計算
	//	worldTransform_.rotation_.x = std::atan2(move.y, lengthXZ);
	//}
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	// 移動
	//worldTransform_.translation_ = worldTransform_.translation_ + move;
	worldTransform_.translation_ = move;


	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection* viewProjection) { 
	models_[kModelIndexBody]->Draw(worldTransformBody_, *viewProjection); 
}
