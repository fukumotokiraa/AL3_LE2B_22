#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include<imgui.h>
#include"PlayerBullet.h"
#include<list>
#include"Calculation.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle /*ViewProjection* viewProjection*/);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void Attack();

	~Player();



private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	//ViewProjection* viewProjection_ = nullptr;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
};