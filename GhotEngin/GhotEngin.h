#pragma once
#include "Window.h"
#include "DirectX12.h"
#include "ShaderCompile.h"
#include "GameScene.h"
#include "GraphicsPipeline.h"
#include "ImguiManager.h"
#include "TextureManager.h"
#include "Input.h"

class GhotEngin
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

private:

	Window* win_;
	DirectX12* dxCommon_;
	ImGuiManager* imguiManager_;
	GameScene* gameScene_;
};

