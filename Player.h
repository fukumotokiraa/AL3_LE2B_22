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
#include<optional>

class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	void BehaviorRootInitialize();

	void BehaviorAttackInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

private:
	enum class Behavior {
		kRoot,
		kAttack,
	};
	uint32_t textureHandle_ = 0; 
	const ViewProjection* viewProjection_ = nullptr;
	float floatingParameter_ = 0.0f;
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	bool isAttack = false;
	bool isPause = false;
	float attackAngle = 0.0f;
	int pauseTime = 10;

	int kModelIndexBody = 0;
	int kModelIndexHead = 1;
	int kModelIndexL_arm = 2;
	int kModelIndexR_arm = 3;
	int kModelIndexHammer = 4;

	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	Model* modelHammer_ = nullptr;

	uint32_t textureBody_ = 0;
	uint32_t textureHead_ = 0;
	uint32_t textureL_arm_ = 0;
	uint32_t textureR_arm_ = 0;
	uint32_t textureHammer_ = 0;

	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;

};
