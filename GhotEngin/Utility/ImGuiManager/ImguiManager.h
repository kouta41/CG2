#pragma once

#include "DirectX12.h"
#include "WinApp.h"
#include <d3d12.h>
#include "imgui.h"

#include <Windows.h>
#include <cstdint>
#include <d3d12.h>

class WinApp;
class DirectXCommon;

class ImGuiManager {
public:

    static ImGuiManager* GetInstance();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize();

    /// <summary>
    /// ImGui受付開始
    /// </summary>
    void Begin();

    /// <summary>
    /// ImGui受付終了
    /// </summary>
    void End();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();


private:
    ImGuiManager() = default;
    ~ImGuiManager() = default;
    ImGuiManager(const ImGuiManager&) = delete;
    const ImGuiManager& operator=(const ImGuiManager&) = delete;

#ifdef _DEBUG
    // DirectX基盤インスタンス（借りてくる）
    DirectXCommon* dxCommon_ = nullptr;
#endif

};

