#pragma once
#include <Windows.h>
#include <cstdint>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include "StringUtility.h"
#include "Window.h"

class DirectX12 {
public: // メンバ関数

	// シングルトンインスタンスの取得
	static DirectX12* GetInstance();

	// デバイスの取得
	ID3D12Device* GetDevice() { return device_.Get(); }

	// 描画コマンドリストの取得
	static ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	DXGI_SWAP_CHAIN_DESC1 GetBufferCount() { return swapChainDesc; }

	ID3D12DescriptorHeap* GetSRV() { return srvHeap_.Get(); }


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Window* win);

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
	DirectX12() = default;
	~DirectX12() = default;
	DirectX12(const DirectX12&) = delete;
	const DirectX12& operator=(const DirectX12&) = delete;

private: // メンバ関数
	Window* win_;
	// FPS固定初期化
	void InitializeFixFPS();
	// FPS固定更新
	void UpdateFixFPS();

private:
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
	// 記録時間	(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};