#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) { 
	models_ = models;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

void BaseCharacter::Update() { worldTransform_.UpdateMatrix(); }

void BaseCharacter::Draw() {
	for (Model*  model:models_) {
		model->Draw(worldTransform_, viewProjection_);
	}
}
