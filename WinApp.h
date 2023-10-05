#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>

class DirectXCommon;

class WinApp {
public:
	WinApp(const wchar_t* title);
	~WinApp();

	WNDCLASS wc{};
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	int32_t GetkClientWidth() { return kClientWidth; }
	int32_t GetkClientHeight() { return kClientHeight; }
	HWND Gethwnd() { return hwnd; }

private:
	// クライアント領域のサイズ
	int32_t kClientWidth = 1280;
	int32_t kClientHeight = 720;

	HWND hwnd = nullptr;
	ID3D12Debug1* debugController = nullptr;

};