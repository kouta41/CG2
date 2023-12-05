#pragma once

#include "DirectX12.h"
#include "Window.h"
#include <d3d12.h>
#include "imgui.h"

#include <Windows.h>
#include <cstdint>
#include <d3d12.h>

#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

class Window;

class DirectX12;

class ImGuiManager {
public:

    static ImGuiManager* GetInstance();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Window* Win, DirectX12* dxCommon);

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
    DirectX12* dxCommon_ = nullptr;
#endif

};

