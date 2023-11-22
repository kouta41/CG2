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
#include "StringUtility.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

class DirectXCommon {
public:

	static DirectXCommon* GetInstance();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win);

	/// <summary>
	/// 描画前
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後
	/// </summary>
	void PostDraw();


	// DXGI初期化
	void InitializeDxgi();

	// コマンド
	void InitializeCommand();

	// スワップチェーン
	void CreateSwapChain();

	// descriptorheap生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible);

	// RTV,SRV作成
	void CreateRenderTargetView();

	// fence作成
	void CreateFence();

	// 深度バッファのクリア
	void ClearDepthBuffer();

	/// 深度バッファ生成
	void CreateDepthBuffer();
	

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	// Accessor
	static ID3D12GraphicsCommandList* GetCommandList_() { return commandList_.Get(); }
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_.Get(); }
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() { return swapChainDesc; }
	ID3D12DescriptorHeap* GetsrvDescriptorHeap_() { return srvDescriptorHeap_.Get(); }



private:

	WinApp* winApp_;

	// DirectX3D関連
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	HANDLE fenceEvent_;
	D3D12_RESOURCE_BARRIER barrier{};
	HRESULT hr_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	UINT backBufferIndex_;

};
