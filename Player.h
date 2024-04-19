#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Player {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t SpriteUvChecker_ = 0; 

};
