#pragma once
#include <Windows.h>
#include <numbers>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <cstdint>
#include "Utility.h"
#include "DirectX12.h"
#include "Mesh.h"
#include "WinApp.h"
#include "ImguiManege.h"

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
	void DrawSprite(DirectXCommon* dir_);
	void DrawSphere(DirectXCommon* dir_);

	// 解放
	void Release();

	void CreateVertexResource(DirectXCommon* dir_, Vector4* pos);
	void CreateMaterialResource(DirectXCommon* dir_);
	void CreateWVPResource(DirectXCommon* dir_);
	void Create2DSpriteResource(DirectXCommon* dir_);
	void CreateSphereResoure(DirectXCommon* dir_);
	void LoadTexture(DirectXCommon* dir_);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbytes);
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	Matrix4x4* GetwvpData() { return wvpData; }
	Vector4* GetmaterialData() { return materialData; }

	Transform transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

private:
	WinApp* window_ = nullptr;

	

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};


	ID3D12Resource* depthStencilResource;


#pragma region TriAngle
	//バーテックスリソース
	ID3D12Resource* vertexResource = nullptr;
	//頂点データ
	VertexData* vertexData = nullptr;
	//バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};

	//マテリアルリソース
	ID3D12Resource* materialResource = nullptr;
	//色データ
	Vector4* materialData = nullptr;
	//WVPリソース
	ID3D12Resource* wvpResource = nullptr;
	//WVPデータ
	Matrix4x4* wvpData = nullptr;
	//テクスチャデータ
	ID3D12Resource* textureResource = nullptr;
#pragma endregion 三角形

#pragma region sprite
	//Sprite用頂点データ
	ID3D12Resource* vertexResourceSprite = nullptr;
	//Sprite用頂点データ
	VertexData* vertexDataSprite = nullptr;
	//Sprite用バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	//Sprite用WVPリソース
	ID3D12Resource* transformationMatrixResourceSprite = nullptr;
	//Sprite用WVPデータ
	Matrix4x4* transformationMatrixDataSprite = nullptr;
#pragma endregion スプライト

#pragma region Sphere
	const int kSubdivision = 16;
	//Sphere用頂点データ
	ID3D12Resource* vertexResourceSphere = nullptr;
	//Sphere用頂点データ
	VertexData* vertexDataSphere = nullptr;
	//Sphere用バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};
	//マテリアルリソース
	ID3D12Resource* materialResourceSphere = nullptr;
	//色データ
	Material* materialDataSphere = nullptr;
	//Sphere用WVPリソース
	ID3D12Resource* transformationMatrixResourceSphere = nullptr;
	//Sphere用WVPデータ
	TransformationMatrix* transformationMatrixDataSphere = nullptr;
#pragma endregion 球

#pragma region Light
	ID3D12Resource* directionalLightResource = nullptr;
	DirectionalLight* directionalLightData = nullptr;
#pragma endregion ライト

	//descriptorHandle
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;


	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2;

	//textureの切り替え
	bool useMonsterBall = true;

	HRESULT hr_;
	Sphere sphere{
				{0.0f,0.0f,0.0f},
				16
	};
	//Transform transform_={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSphere = { {0.1f,0.1f,0.1f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, };
};
