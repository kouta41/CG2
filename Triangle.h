#pragma once
#include <Windows.h>
#include "Utility.h"

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
	
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	ID3DBlob* errorBlob_;
	IDxcIncludeHandler* includeHandler_;
	ID3DBlob* signatureBlob_;
	ID3D12RootSignature* rootSignature_;
	ID3D12Device* device_;
	ID3D12PipelineState* graphicsPipelineState_;
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	HRESULT hr_;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	D3D12_VERTEX_BUFFER_VIEW materialBufferView_{};

	Matrix4x4* wvpDate_;
	Vector4* vertexData_;
	Vector4* materialDate_;
	ID3D12Resource* materialResource_;
	ID3D12Resource* vertexResource_;
	ID3D12Resource* wvpResource_;
	ID3D12Resource* wvpmResource_;
	Transform transform_;
	Transform cameraTransfrom_{};
	ID3D12GraphicsCommandList* commandList_;
	Matrix4x4* transformationMatrixData_;

	Vector4 color = { 1.0f,0.0f,0.0f,1.0f };

	ID3D12Resource* CreatBufferResource(ID3D12Device* device_, size_t sizeInBytes);

public:
	Triangle();
	~Triangle();

	
	Vector4 triangleData[10];

	void Init(DirectX12* dx12Common);

	void Draw(Vector4 triangleData[10]);

	
	void Release();
	
	HRESULT Gethr_() { return hr_; }
	Vector4 Getcolor() { return color; }
};


