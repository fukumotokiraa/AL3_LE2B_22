#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "sstream"

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

	//void Fire();

	void CheckAllCollisions();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void GenerateEnemy(const Vector3 position);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	const std::list<EnemyBullet*>& GetEnemyBullet() const { return enemyBullets_; }

	const std::list<Enemy*>& GetEnemy() const { return enemies_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	WorldTransform worldTransform_;
	bool isWait_ = false;
	int waitTimer_ = 0;
	std::list<EnemyBullet*> enemyBullets_;
	std::list<Enemy*> enemies_;
	std::stringstream enemyPopCommands;

	// uint32_t SoundMokugyo_ = 0;
	// uint32_t VoiceMokugyo_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};