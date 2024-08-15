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

class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

private:
	uint32_t textureHandle_ = 0; 
	const ViewProjection* viewProjection_ = nullptr;
	float floatingParameter_ = 0.0f;


	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	uint32_t textureBody_ = 0;
	uint32_t textureHead_ = 0;
	uint32_t textureL_arm_ = 0;
	uint32_t textureR_arm_ = 0;

	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

};
