#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;
	model_ = model;
}

void Player::Update() { 
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
