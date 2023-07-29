#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert> 
#include <dxgidebug.h>

#include "ConvertString.h"
#include "Utility.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class WinApp;
class Triangle;

class DirectX12 {
private:

	IDXGIFactory7* dxgiFactory_;
	IDXGIAdapter4* useAdapter_;
	ID3D12Device* device_;
	ID3D12CommandQueue* commandQueue_;
	ID3D12CommandAllocator* commandAllocator_;
	ID3D12GraphicsCommandList* commandList_;
	IDXGISwapChain4* swapChain_;
	ID3D12DescriptorHeap* rtvDescriptorHeap_;
	ID3D12DescriptorHeap* srvDescriptorHeap_;
	ID3D12Resource* swapChainResources_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_RESOURCE_BARRIER barrier{};
	ID3D12InfoQueue* infoQueue_;
	ID3D12Fence* fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	IDXGIDebug1* debug_;
	HRESULT hr_;
	HWND hwnd_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	ID3D12Debug1* debugController_;

public:

	DirectX12();
	~DirectX12();

	void Init(WinApp* winApp);

	void Update();

	void Draw();

	void Release();


	HRESULT Gethr_() { return hr_; }
	ID3D12Device* Getdevice() { return device_; }
	ID3D12GraphicsCommandList* GetcommandList() { return commandList_; }
	ID3D12DescriptorHeap* GetsrvDescriptorHeap_() { return srvDescriptorHeap_; }
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() { return swapChainDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetrtvDesc() { return rtvDesc; }

};