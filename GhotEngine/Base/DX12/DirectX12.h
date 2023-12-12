#pragma once

#include <Windows.h>
#include <cstdint>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <chrono>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#include "Window.h"

class DirectXCommon {
public: // メンバ関数

	// シングルトンインスタンスの取得
	static DirectXCommon* GetInstance();

	// デバイスの取得
	ID3D12Device* GetDevice() { return device_.Get(); }

	// 描画コマンドリストの取得
	static ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	DXGI_SWAP_CHAIN_DESC1 GetBufferCount() { return swapChainDesc; }

	ID3D12DescriptorHeap* GetSRV() { return srvHeap_.Get(); }


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

private: // メンバ関数
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

private: // メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

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