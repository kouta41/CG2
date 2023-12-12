#pragma once
#include <array>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <DirectXTex.h>
#include "StringUtility.h"
#include "DirectX12.h"
#define TEXTURE_LOAD_MAX 128

struct descSize {
	//size
	uint32_t SRV = 0;
	uint32_t RTV = 0;
	uint32_t DSV = 0;
};

class TextureManager {
public:
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 読み込み
	/// </summary>
	static uint32_t Load(const std::string& fileName);

	// get
	static uint32_t GetIndex() { return TextureManager::GetInstance()->index_; }
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t texHandle);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	/*void ResetAllTex();*/


private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	static void  LoadTex(const std::string& filePath, uint32_t index);

	static ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	static void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

private: // メンバ変数

	uint32_t index_ = 0;

	descSize size = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> texResource[TEXTURE_LOAD_MAX] = {};
	// シェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV[TEXTURE_LOAD_MAX] = {};
	// シェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV[TEXTURE_LOAD_MAX] = {};

};