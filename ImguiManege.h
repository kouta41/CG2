#pragma once
#include <Windows.h>
#include"ConvertString.h"

#include <Windows.h>
#include"ConvertString.h"
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
#include <dxgi1_6.h>


class Imgui {
private:
	ID3D12DescriptorHeap* rtvDescriptorHeap;
	ID3D12DescriptorHeap* srvDescriptorHeap;
	ID3D12Device* device_;
	ID3D12GraphicsCommandList* commandList_;
	HWND hwnd;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
public:
	Imgui();

	void Init();

	void Draw();

	ID3D12DescriptorHeap* GetsrvDescriptorHeap_() { return srvDescriptorHeap; }
};