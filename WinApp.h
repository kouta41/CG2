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

	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static inline HWND hwnd = nullptr;
	static inline ID3D12Debug1* debugController = nullptr;
};