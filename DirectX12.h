#pragma once
#include <Windows.h>

#include<string>
#include<format>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert> 
#include<dxgidebug.h>

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
	ID3D12Resource* swapChainResources_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_RESOURCE_BARRIER barrier{};
	ID3D12InfoQueue* infoQueue_;
	ID3D12Fence* fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	IDXGIDebug1* debug_;
	HRESULT hr_;

public:

	DirectX12();
	~DirectX12();

	void Init();

	void Initdxcommand(WinApp* winApp);

	void Loadcommand();

	void CreateFence();

	void DirectXRelease(WinApp* winApp);


	HRESULT Gethr_() { return hr_; }
	ID3D12Device* Getdevice() { return device_; }
	ID3D12GraphicsCommandList* GetcommandList() { return commandList_; }
};