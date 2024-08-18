#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"

class BaseCharacter {
protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(ViewProjection* viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
};
