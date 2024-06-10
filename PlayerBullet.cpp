#include "PlayerBullet.h"
#include"cassert"

void PlayerBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = positon;

	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	//座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0){
		isDead_ = true;
	}

	//行列を更新
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}
