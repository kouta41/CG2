#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();


	model_.reset(Model::CreateObj("cube.obj"));

	texHandle_= TextureManager::Load("resources/goal.png");
	model_->SetTexHandle(texHandle_);


}

// 更新
void GameScene::Update() {


}

// 描画
void GameScene::Draw() {

}
