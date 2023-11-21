#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>

#include "DirectX12.h"
#include "Utility.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lPram);

class DirectXCommon;

class WinApp {

public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int32_t kWindowWidth = 1280; // 横
	static const int32_t kWindowHeight = 720; // 縦

public:
	// シングルトンインスタンスの取得
	static WinApp* GetInstance();

	WinApp(const wchar_t* title);
	~WinApp();




	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	// ウィンドウハンドルの取得

	HWND Gethwnd() { return hwnd; }

	// ウィンドウクラスの取得
	WNDCLASS GetWc() const { return wc; }

private:
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private:
	
	WNDCLASS wc{};
	HWND hwnd = nullptr;
	ID3D12Debug1* debugController = nullptr;

};