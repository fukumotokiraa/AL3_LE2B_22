#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<ImGuiManager.h>
#include<PrimitiveDrawer.h>
#include"AxisIndicator.h"


GameScene::GameScene() {}

GameScene::~GameScene() { 
	//delete sprite_;
	//delete model_;
	//delete debugCamera_; 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	/*SpriteMario_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(SpriteMario_, {100, 50});
	ModelMario_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	SoundMokugyo_ = audio_->LoadWave("mokugyo.wav");
	audio_->PlayWave(SoundMokugyo_);
	VoiceMokugyo_ = audio_->PlayWave(SoundMokugyo_, true);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());*/
}

void GameScene::Update() {
//	Vector2 position = sprite_->GetPosition();
//	position.x += 2.0f;
//	position.y += 1.0f;
//	sprite_->SetPosition(position);
//	if (input_->TriggerKey(DIK_SPACE)) {
//		audio_->StopWave(VoiceMokugyo_);
//	}
//#ifdef DEBUG
//	ImGui::Begin("Debug1");
//	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
//	ImGui::InputFloat3("InputFloat3", inputFloat3);
//	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
//	ImGui::ShowDemoWindow();
//	ImGui::End();
//#endif
//	debugCamera_->Update();

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

	/*model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), ModelMario_);*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/*sprite_->Draw();*/

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
