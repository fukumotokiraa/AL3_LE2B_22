#include "Player.h"
#include <cassert>
#include"GameScene.h"



void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {
	assert(model);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	textureHandle_ = textureHandle;
	model_ = model;
	input_ = Input::GetInstance();
	worldTransform_.translation_ = Add(worldTransform_.translation_, playerPosition);
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("./Resources/Reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {255, 255, 255, 255}, {0.5f, 0.5f});
}

void Player::Update() {
	//// 行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	//デスフラグの立った玉を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	////押した方向で移動ベクトルを変更(左右)
	//if (input_->PushKey(DIK_LEFT)) {
	//	move.x -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move.x += kCharacterSpeed;
	//}
	////押した方向で移動ベクトルを変更(上下)
	//if (input_->PushKey(DIK_DOWN)) {
	//	move.y -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_UP)) {
	//	move.y += kCharacterSpeed;
	//}



	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0,joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	ImGui::Begin(" ");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::End();

	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Rotate();


	Attack();

	for (PlayerBullet*bullet : bullets_) {
		bullet->Update();
	}

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		//自機の回転行列の反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		//ベクトルの長さを整える
		offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
		//worldTransform3DReticle_.UpdateWorld();
		worldTransform3DReticle_.UpdateMatrix();
	}



	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{


		//POINT mousePosition;
		////マウス座標(スクリーン座標)を取得する
		//GetCursorPos(&mousePosition);

		////クライアントエリア座標に変換する
		//HWND hwnd = WinApp::GetInstance()->GetHwnd();
		//ScreenToClient(hwnd, &mousePosition);

		////マウス座標を2Dレティクルのスプライトに代入する
		//sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

		// スプライトの現在座標の取得
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		// ジョイスティック状態取得
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
			// スプライトの座標変更を反映
			sprite2DReticle_->SetPosition(spritePosition);
		}

		// ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = Multiply4x4(Multiply4x4(viewProjection_.matView, viewProjection_.matProjection), matViewport);
		//合成行列の逆行列を計算する
		Matrix4x4 matInverseViewProjectionViewport = Inverse(matViewProjectionViewport);

		//スクリーン座標
		Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
		Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);

		//スクリーン座標系からワールド座標系へ
		posNear = Transform(posNear, matInverseViewProjectionViewport);
		posFar = Transform(posFar, matInverseViewProjectionViewport);

		//マウスレイの方向
		Vector3 mouseDirection = Subtract(posFar, posNear);
		mouseDirection = Normalize(mouseDirection);
		//カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 300.0f;
		worldTransform3DReticle_.translation_ = Add(posNear, Scale(mouseDirection, kDistanceTestObject));

		ImGui::Begin("Player");
		ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
		ImGui::Text("Near:(%+.2f,%+.2f,%+.2f,)", posNear.x, posNear.y, posNear.z);
		ImGui::Text("Far:(%+.2f,%+.2f,%+.2f,)", posFar.x, posFar.y, posFar.z);
		ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f,)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
		ImGui::End();

		worldTransform3DReticle_.UpdateMatrix();
		worldTransform3DReticle_.UpdateWorld();
	}
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = worldTransform3DReticle_.translation_;
		// ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = Multiply4x4(Multiply4x4(viewProjection_.matView, viewProjection_.matProjection), matViewport);
		// ワールド→スクリーン座標変換
		positionReticle = Transform(positionReticle, matViewProjectionViewport);
		// スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	//3Dモデルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::Rotate() {
	//回転速さ
	const float kRotSpeed = 0.02f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//玉の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(GetReticleWorldPosition(), GetWorldPosition());
		velocity = Multiply(1.0f, Normalize(velocity));
		//玉を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//玉を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
}

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

Vector3 Player::GetWorldPosition() {
	Vector3 WorldPos;//ワールド座標を入れる変数
	//ワールド行列の平行移動成分を取得(ワールド座標)
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}

Vector3 Player::GetReticleWorldPosition() {
	Vector3 WorldPos; // ワールド座標を入れる変数
	// ワールド行列の平行移動成分を取得(ワールド座標)
	WorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	WorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	WorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return WorldPos;
}