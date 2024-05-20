#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include<cassert>
#include<numbers>
#include<input/Input.h>
#include<algorithm>
#include"MapChipField.h"
#include<imgui.h>

class MapChipField;

class Player {
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttennuation = 0.01f;
	static inline const float kLimitRunSpeed = 1.0f;
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間＜秒＞
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 5.0f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 1.0f;

	bool isLimitUpper_ = false;
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.6f;
	static inline const float kHeight = 1.6f;

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool wall = false;
		// 移動量
		Vector3 movement = {};
	};
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner //要素数
	};
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	static inline const float kBlank = 0.0f;


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& Position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void Move();

	void MapCollision(CollisionMapInfo& info);

	void TopCollision(CollisionMapInfo& info);

	void ResultMove(CollisionMapInfo& info);

	void Ceiling(CollisionMapInfo& info);
};