#pragma once
#include"Model.h"
#include"WorldTransform.h"

class BaseCharacter {
protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

public:
	virtual void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection);

	virtual void Update();

	virtual void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
};
