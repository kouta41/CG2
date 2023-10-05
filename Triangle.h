#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <cstdint>
#include "Utility.h"
#include "DirectX12.h"
#include "Mesh.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Triangle {
public:

	// 初期化
	void Initialize(DirectXCommon* dir_, Vector4* pos);
	// 読み込み
	void Update();
	// 描画
	void Draw(DirectXCommon* dir_);
	// 解放
	void Release();

	void CreateVertexResource(DirectXCommon* dir_, Vector4* pos);
	void CreateMaterialResource(DirectXCommon* dir_);
	void CreateWVPResource(DirectXCommon* dir_);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbytes);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

public:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};

	ID3D12Resource* vertexResource;
	ID3D12Resource* materialResource;
	ID3D12Resource* wvpResource;

	VertexData* vertexData;
	Vector4* materialData;
	Matrix4x4* wvpData;

	ID3D12Resource* textureResource;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	static inline HRESULT hr_;
};
