#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete modelBlock_;
	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;
	delete modelPlayerCube_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	
	player_ = new Player();
	modelBlock_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelPlayerCube_ = Model::CreateFromOBJ("PlayerCube", true);




	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, textureHandle_, &viewProjection_);

	//座標をマップチップ単位で指定
	Vector3 PlayerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);

	player_->Initialize(modelPlayerCube_, &viewProjection_, PlayerPosition);

	//ビュープロジェクション
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();
}

void GameScene::Update() { 
	player_->Update();
	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		    isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewMatrix();
		viewProjection_.matProjection = debugCamera_->GetProjectionMatrix();
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
	//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
		
	skydome_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw();
	//ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	// 要素数
	const uint32_t kNumBlockVirtical = 20;
	const uint32_t kNumBlockHorizontal = 100;
	//// ブロック１個分の横幅
	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;
	// 要素数を変更する
	// 列数を設定
	worldTransformBlocks_.resize(mapChipField_->kNumBlockVirtical);
	for (uint32_t i = 0; i < mapChipField_->kNumBlockVirtical; i++) {
		// １列の要素数を設定
		worldTransformBlocks_[i].resize(mapChipField_->kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
