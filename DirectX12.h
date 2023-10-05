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
#include"externals/DirectXTex/d3dx12.h"


#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class WinApp;
class Triangle;

class DirectX12 {
private:
	WinApp* winApp_;

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
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	IDxcBlob* vertexShaderBlob_ = nullptr;
	IDxcBlob* pixelShaderBlob_ = nullptr;
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	//ビューポート
	D3D12_VIEWPORT viewport{};
	//シザー
	D3D12_RECT scissorRect{};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};

#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
#endif

	//プライベート関数
	ID3D12DescriptorHeap* CreatDescriptorHeap(ID3D12Device* device,D3D12_DESCRIPTOR_HEAP_TYPE heapType,UINT numDescriptors,bool shaderVisible);
	void MakeDXGIFactory();
	void MakeD3D12Device();
	void MakeCommandQueue();
	void MakeCommandAllocator();
	void MakeCommandList();
	void MakeSwapChain();
	void MakeDescriptorHeap();
	void MakeFence();
	void MakeDXC();
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler);
	//PSO
	void MakeRootSignature();
	void MakeInputLayOut();
	void MakeBlendState();
	void MakeRasterizarState();
	void MakeShaderCompile();
	void MakePipelineStateObject();

	void MakeViewport();
	void MakeScissor();


public:

	DirectX12();
	~DirectX12();

	void Init(WinApp* winApp);

	void PreView();

	void PostView();

	void Release();


	HRESULT Gethr_() { return hr_; }
	ID3D12Device* Getdevice() { return device_; }
	ID3D12GraphicsCommandList* GetcommandList() { return commandList_; }
	ID3D12DescriptorHeap* GetsrvDescriptorHeap_() { return srvDescriptorHeap_; }
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() { return swapChainDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetrtvDesc() { return rtvDesc; }
	D3D12_GPU_DESCRIPTOR_HANDLE GettextureSrvHandleGPU() { return textureSrvHandleGPU; }

};