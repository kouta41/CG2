#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp {
public:
	//WinApp(const wchar_t* title);
	

	//static const int kClientWidth = 1280;
	//static const int kClientHeight = 720;
	static void Initirize(const wchar_t* title);
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	HWND hwnd = nullptr;
};