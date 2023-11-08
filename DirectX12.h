#pragma once
#include <Windows.h>
#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cassert>
#include <string>
#include <format>
#include <cstdint>
#include "WinApp.h"
#include "ConvertString.h"
#include "Utility.h"

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


	

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	// Accessor
	ID3D12GraphicsCommandList* GetCommandList_() { return commandList_.Get(); }
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() { return swapChainDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetrtvDesc() { return rtvDesc; }
	ID3D12DescriptorHeap* GetsrvDescriptorHeap_() { return srvDescriptorHeap_.Get(); }
	ID3D12DescriptorHeap* GetdsvDescriptorHeap_() { return dsvDescriptorHeap_.Get(); }
	ID3D12DescriptorHeap* GetrtvDescriptorHeap_() { return 	rtvDescriptorHeap_.Get(); }


private:

	WinApp* window_;

	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_RESOURCE_BARRIER barrier{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent = 0;
	HRESULT hr_;


};
