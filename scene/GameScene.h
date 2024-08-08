#pragma once

#include"Player.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include"ViewProjection.h"
#include "WorldTransform.h"
#include"Skydome.h"
#include "Ground.h"
#include"FollowCamera.h"
#include<vector>
#include<DebugCamera.h>
#include<AxisIndicator.h>
#include<memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0; 
	std::unique_ptr<Model> model_ = nullptr;
	ViewProjection viewProjection_;
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;


	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;


	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;




	



	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
