#pragma once
#include "ViewProjection.h"



//前方宣言
class Player;

class CameraController {
private:
	ViewProjection* viewProjection_ = nullptr;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -50.0f};
	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	Rect movableArea_ = {0, 100, 10, 50};
	//カメラの目標座標
	Vector3 targetCoordinates_ = targetOffset_;
	static inline const float kInterpolationRate = 0.5f;
	static inline const float kVelocityBias = 5.0f;

	static inline const Rect margin_ = {-200, 200, -200, 200};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area; }

	Rect& GetMovableArea() { return movableArea_; }

};
