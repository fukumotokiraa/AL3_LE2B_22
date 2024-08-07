#include "Ground.h"
#include<cassert>

void Ground::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	// textureHandle_ = textureHandle;
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/Ground/ground.png");
}

void Ground::Update() {
	worldTransform_.scale_ = {500, 500, 500};
	worldTransform_.UpdateMatrix();
}

void Ground::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
