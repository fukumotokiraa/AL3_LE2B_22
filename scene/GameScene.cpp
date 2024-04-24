#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

//Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
//	return {scale.x, 0, 0, 0, 
//			0, scale.y, 0, 0,
//			0, 0, scale.z, 0,
//			0, 0, 0, 1
//	}; 
//}
//Matrix4x4 MakeRotateXMatrix(float radian) {
//	return {1, 0, 0, 0,
//			0, cosf(radian), sinf(radian), 0,
//			0, -sinf(radian), cosf(radian), 0,
//			0, 0, 0, 1
//	}; 
//}
//Matrix4x4 MakeRotateYMatrix(float radian) {
//	return {cosf(radian), 0, -sinf(radian), 0,
//			0, 1, 0, 0,
//			sinf(radian), 0, cosf(radian), 0,
//			0, 0, 0, 1
//	}; 
//}
//Matrix4x4 MakeRotateZMatrix(float radian) {
//	return {cosf(radian), sinf(radian), 0, 0,
//			-sinf(radian), cosf(radian), 0, 0,
//			0, 0, 1, 0,
//			0, 0, 0, 1
//	};
//}
//Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
//	return {
//	    m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
//	    m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
//	    m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
//	    m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
//	    m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
//	    m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
//	    m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
//	    m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],
//	    m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
//	    m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
//	    m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
//	    m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
//	    m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
//	    m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
//	    m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
//	    m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
//	};
//};
//Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
//	return {1, 0, 0, 0, 
//			0, 1, 0, 0,
//			0, 0, 0, 0,
//			translate.x, translate.y, translate.z, 1
//	}; 
//};
//Matrix4x4 MakeAffineMatrix(Vector3& radian ,const Vector3& scale,const Vector3& translate) {
//	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(radian.x);
//	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(radian.y);
//	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(radian.z);
//	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
//	return {
//	    scale.x * rotateMatrix.m[0][0],
//	    scale.x * rotateMatrix.m[0][1],
//	    scale.x * rotateMatrix.m[0][2],
//	    0,
//	    scale.y * rotateMatrix.m[1][0],
//	    scale.y * rotateMatrix.m[1][1],
//	    scale.y * rotateMatrix.m[1][2],
//	    0,
//	    scale.z * rotateMatrix.m[2][0],
//	    scale.z * rotateMatrix.m[2][1],
//	    scale.z * rotateMatrix.m[2][2],
//	    0,
//	    translate.x,
//	    translate.y,
//	    translate.z,
//	    1};
//}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete model_;
	delete modelBlock_;
	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	viewProjection_.Initialize();
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);
	modelBlock_ = Model::Create();
	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック１個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	//要素数を変更する
	//列数を設定
	worldTransformBlocks_.resize(kNumBlockHorizontal);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		//１列の要素数を設定
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			if (i == 5 && j == 10 || i == 5 && j == 5)
			{
				worldTransformBlocks_[i][j] = NULL;
			}
		}
	}

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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
	//player_->Draw();
	//ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

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
