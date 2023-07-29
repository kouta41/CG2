#pragma once
#include <Windows.h>
#include<string>
#include"ImguiManege.h"
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include"imgui.h"
#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class WinApp {

private:
	/*--ウィンドウクラスを登録する---*/
	WNDCLASS wc_;

	//クライアント領域サイズ
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	//windows Title
	std::wstring title_;
	//ウィンドウサイズを表す構造佁E
	RECT wrc_;

	//ウィンドウ
	HWND hwnd_;

	ID3D12Debug1* debugController_;



public:

	WinApp(
		int width,
		int height,
		std::wstring title
	);

	~WinApp();

	//ウィンドウクラスを登録
	void RegistrateWindowClass();

	static LRESULT WindowProc(HWND hwnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

	//ウィンドウの生成
	void CreateAppWindow();


	//ウィンドウを表示する
	void ShowAppWindow();

	//メッセージの処理
	bool ProcessMessage();

	HWND Gethwnd_() { return this->hwnd_; }

	ID3D12Debug1* GetdebugController_() { return debugController_; }
};