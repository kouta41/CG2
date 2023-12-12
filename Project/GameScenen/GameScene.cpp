#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_= TextureManager::Load("resources/uvChecker.png");
	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);


}

// 更新
void GameScene::Update() {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		worldTransform_.translate.x += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		worldTransform_.translate.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		worldTransform_.translate.z += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		worldTransform_.translate.z -= 0.1f;
	}
}

// 描画
void GameScene::Draw() {
	model_->Draw(worldTransform_, viewProjection_);

}
