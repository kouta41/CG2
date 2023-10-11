#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <cstdint>
#include "Utility.h"
#include "DirectX12.h"
#include "Mesh.h"
#include "WinApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Triangle {
public:

	// 初期化
	void Initialize(DirectXCommon* dir_, Vector4* pos,WinApp* window);
	// 読み込み
	void Update();
	// 描画
	void Draw(DirectXCommon* dir_);
	void SpriteDraw(DirectXCommon* dir_);

	// 解放
	void Release();

	void CreateVertexResource(DirectXCommon* dir_, Vector4* pos);
	void CreateMaterialResource(DirectXCommon* dir_);
	void CreateWVPResource(DirectXCommon* dir_);
	void Create2DSpriteResource(DirectXCommon* dir_);


	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbytes);
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	Matrix4x4* GetwvpData() { return wvpData; }
	Vector4* GetmaterialData() { return materialData; }
	ID3D12Resource* GetdepthStencilResource;

private:
	WinApp* window_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	ID3D12Resource* vertexResource;
	ID3D12Resource* materialResource;
	ID3D12Resource* wvpResource;

	ID3D12Resource* textureResource;
	ID3D12Resource* depthStencilResource;

	ID3D12Resource* vertexResourceSprite;
	ID3D12Resource* transformationMatrixResourceSprite;

	VertexData* vertexData;
	VertexData* vertexDataSprite;
	Vector4* materialData;
	Matrix4x4* wvpData;
	Matrix4x4* transformationMatrixDataSprite;


	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	HRESULT hr_;

	Transform transform_={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

};
