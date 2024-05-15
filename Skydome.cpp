#include "Skydome.h"
#include<cassert>

void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	textureHandle_ = textureHandle;
	model_ = model;
}

void Skydome::Update() {
	worldTransform_.scale_ = {150, 150, 150};
	worldTransform_.UpdateMatrix(); 
}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}
