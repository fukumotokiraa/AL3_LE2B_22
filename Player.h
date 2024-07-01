#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include<imgui.h>
#include"PlayerBullet.h"
#include<list>
#include"Calculation.h"
#include"Sprite.h"
#include"WinApp.h"
#include"ViewProjection.h"

class GameScene;

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 playerPosition);

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

	void OnCollision();//衝突を検出したら呼び出されるコールバック関数

	void SetParent(const WorldTransform* parent);

	void DrawUI();

	~Player();

	Vector3 GetWorldPosition();

	Vector3 GetReticleWorldPosition();

	const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	//ViewProjection* viewProjection_ = nullptr;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;
	GameScene* gameScene_ = nullptr;
	ViewProjection viewProjection_;
};