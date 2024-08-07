#pragma once
#include"ViewProjection.h"
#include "WorldTransform.h"
#include"Calculation.h"
#include"Input.h"

class FollowCamera {
private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	Matrix4x4 rotationMatrix;

public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

	Matrix4x4 GetRotateMatrix() const { return rotationMatrix; }

};
