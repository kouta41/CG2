#include "GhotEngin.h"

/// <summary>
/// 初期化
/// </summary>
void Engine::Initialize() {
	// リワークチェック(できてるかわからん)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	win_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	// ウィンドウの作成
	win_->CreateGameWindow(L"DirectXGame");
	// DirectX初期化
	dxCommon_->Initialize(win_);
	Input::Initialize();
	GraphicsPipeline::Initialize();
	TextureManager::GetInstance()->Initialize();

	//ゲームシーンの初期化
	gameScene_ = new GameScene();
	gameScene_->Initialize();
	
	// ImGuiの初期化
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(win_, dxCommon_);
}

/// <summary>
/// ゲームループ
/// </summary>
void Engine::Run() {

	// メインループ
	while (true) {
		// メッセージ処理
		if (win_->ProcessMessage()) {
			break;
		}

		// input
		Input::Update();
		//imgui受付開始
		imguiManager_->Begin();

		//ゲームシーンの更新
		gameScene_->Update();
		//imguiManager_->End();

		// 描画前処理
		dxCommon_->PreDraw();

		//ゲームシーンの描画
		gameScene_->Draw();

		imguiManager_->End();

		imguiManager_->Draw();
		// 描画後処理
		dxCommon_->PostDraw();

	}
}

/// <summary>
/// 終了
/// </summary>
int Engine::Finalize() {

	imguiManager_->Finalize();
	// ゲームウィンドウ破棄
	win_->TerminateGameWindow();

	return 0;
}