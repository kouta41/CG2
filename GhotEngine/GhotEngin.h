#pragma once
#include "Window.h"
#include "DirectX12.h"
#include "ShaderCompile.h"
#include "GameScene.h"
#include "GraphicsPipeline.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Input.h"

class Engine {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Run();

	/// <summary>
	/// 終了
	/// </summary>
	int Finalize();

private:
	WinApp* win_;
	DirectXCommon* dxCommon_;
	ImGuiManager* imguiManager_;
};