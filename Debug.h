#pragma once
#include <Windows.h>
#include"ConvertString.h"
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
//extern IMGUI_IMPL_API LRESULT Imgui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Imgui {
private:

	ID3D12DescriptorHeap* rtvDescriptorHeap;
	ID3D12DescriptorHeap* srvDescriptorHeap;
	ID3D12Device* device_;


public:
	Imgui();
	~Imgui();

	void Init();

	void Update();

	void Draw();
};