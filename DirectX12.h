#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <cassert>
#include <string>
#include <format>
#include <cstdint>
#include <Windows.h>
#include "WinApp.h"
#include "ConvertString.h"
#include "Mesh.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

class DirectXCommon {
public:
	// Default Methods
	void Initialize(WinApp* winApp_);
	void Update();
	// void Draw(); //not use
	void Release(WinApp* winApp_);


	// User Methods
	void Fence();
	void Close();

	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);


	// Accessor
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }
	ID3D12Device* GetDevice() { return device_; }

public:
	IDXGIFactory7* dxgiFactory_ = nullptr;
	ID3D12Device* device_ = nullptr;
	IDXGIAdapter4* useAdapter_ = nullptr;
	ID3D12CommandQueue* commandQueue_ = nullptr;
	ID3D12CommandAllocator* commandAllocator_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	IDXGISwapChain4* swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_RESOURCE_BARRIER barrier{};
	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
	ID3D12Resource* swapChainResources[2] = { nullptr };
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent = 0;
	static inline HRESULT hr_;

	//static inline HWND hwnd = nullptr;
};
