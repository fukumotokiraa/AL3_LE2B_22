#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Calculation.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include<imgui.h>
#include <Input.h>
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update();

	void Draw(ViewProjection* viewProjection);

private:
	float angle = 0.0f;

	int kModelIndexBody = 0;

	Model* modelBody_ = nullptr;

	uint32_t textureBody_ = 0;

	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
};
