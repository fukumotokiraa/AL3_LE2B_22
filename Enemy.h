#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "Calculation.h"
#include<imgui.h>

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 position_;
	const float EnemySpeed = 0.5f; // 敵の速度
	Vector3 velocity_;

};
