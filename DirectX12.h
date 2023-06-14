#pragma once
#include <Windows.h>
#include "ConvertString.h"

#include<string>
#include<format>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert> 

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include"WinApp.h"

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


	HRESULT hr_;


	void Log(const std::string& message) {
		OutputDebugStringA(message.c_str());
	}



public:
	//WinApp* winApp = new WinApp(1280, 720, L"CG2");

	DirectX12();
	~DirectX12();

	void Init(WinApp* winApp);

	void DrawdirectX12();


};