#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "Calculation.h"
#include <imgui.h>
#include "EnemyBullet.h"

class Player;//自機クラスの前方宣言

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

	void Approach();

	void Leave();

	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	void OnCollision(); // 衝突を検出したら呼び出されるコールバック関数

	~Enemy();

	Vector3 GetWorldPosition();

	const std::list<EnemyBullet*>& GetBullet() const { return bullets_; }


	static const int kFireInterval = 60;//発射間隔

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 position_;
	float EnemySpeed = 0.5f; // 敵の速度
	const float ApproachSpeed = 0.1f;//接近速度
	const float LeaveSpeed = 1.0f;//離脱速度
	Vector3 velocity_;
	enum class Phase { //行動フェーズ
		Approach, //接近する
		Leave,    //離脱する
	};
	Phase phase_ = Phase::Approach;
	std::list<EnemyBullet*> bullets_;
	int32_t fireTimer_ = 0;
	Player* player_ = nullptr;//自キャラ
	Vector3 DifferenceVector;

};
