#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/Player/tex.png");
}

void Player::Update() { 
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }
