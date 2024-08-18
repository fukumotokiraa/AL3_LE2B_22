#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) { 
	models_ = models;
	worldTransform_.Initialize();
}

void BaseCharacter::Update() { worldTransform_.UpdateMatrix(); }

void BaseCharacter::Draw(ViewProjection* viewProjection) {
	viewProjection->Initialize();
	//viewProjection_ = viewProjection;
	for (Model*  model:models_) {
		model->Draw(worldTransform_, *viewProjection);
	}
}
