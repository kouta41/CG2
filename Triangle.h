#pragma once
#include <Windows.h>
#include "Utility.h"
#include "externals/DirectXTex/DirectXTex.h"

#include<dxcapi.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert> 
#include<dxgidebug.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

class DirectX12;


class Triangle {
private:

	DirectX12* dx12Common_ = nullptr;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
	HRESULT hr_;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Matrix4x4* wvpData_;
	Vector4* vertexData_;
	Vector4* materialData_;
	ID3D12Resource* materialResource_;
	ID3D12Resource* vertexResource_;
	ID3D12Resource* wvpResource_;
	ID3D12Resource* wvpmResource_;
	ID3D12Resource* intermediateResource_ = nullptr;
	Transform transform_;
	Transform cameraTransfrom_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	Matrix4x4* transformationMatrixData_;

	Vector4 color = {1.0f,0.0f,0.0f,1.0f};

	ID3D12Resource* CreatBufferResource(size_t sizeInBytes);
	VertexData* vertexData = nullptr;
	Matrix4x4 ViewMatrix;
	ID3D12Resource* textureResource = nullptr;

	DirectX::ScratchImage ImageFileOpen(const std::string& filePath);
	void MakeVertexBufferView();
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
public:
	Triangle();
	~Triangle();

	
	Vector4 triangleData[10];

	void Init(DirectX12* dx12Common);

	void Draw(Vector4 triangleData[10]);

	
	void Release();

	void LoadTexture(const std::string& filePath);

	HRESULT Gethr_() { return hr_; }
};


