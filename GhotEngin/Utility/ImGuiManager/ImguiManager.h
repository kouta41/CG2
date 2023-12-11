#pragma once

#include <d3d12.h>
#include <imgui.h>
#include <wrl.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


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

private:
    ImGuiManager() = default;
    ~ImGuiManager() = default;
    ImGuiManager(const ImGuiManager&) = delete;
    const ImGuiManager& operator=(const ImGuiManager&) = delete;

    // DirectX基盤インスタンス（借りてくる）
    DirectXCommon* dxCommon_ = nullptr;

};