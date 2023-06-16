#pragma once
#include <Windows.h>
//#include<string>
//#include<format>



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
	struct Vector4 final {
		float x;
		float y;
		float z;
		float w;
	};

	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	ID3DBlob* errorBlob_;
	IDxcIncludeHandler* includeHandler_;
	ID3DBlob* signatureBlob_;
	ID3D12RootSignature* rootSignature_;
	ID3D12Device* device_;
	ID3D12PipelineState* graphicsPipelineState_;
	ID3D12Resource* vertexResource_;
	Vector4* vertexData_;
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;


	HRESULT hr_;

	

public:
	Triangle();
	~Triangle();

	void Init(DirectX12* dx12Common);


	D3D12_VIEWPORT &Getviewport() { return viewport_; }
	D3D12_RECT &GetscissorRect() { return scissorRect_; }
	ID3D12RootSignature* GetrootSignature() { return rootSignature_; }
	ID3D12PipelineState* GetgraphicsPipelineState() { return graphicsPipelineState_; }
	D3D12_VERTEX_BUFFER_VIEW &GetvertexBufferView() { return vertexBufferView_; }

	ID3D12Resource* GetvertexResource() { return vertexResource_; }
	ID3DBlob* GeterrorBlob() { return errorBlob_; }
	ID3DBlob* GetsignatureBlob() { return signatureBlob_; }
	IDxcBlob* GetvertexShaderBlob() { return vertexShaderBlob_; }
	IDxcBlob* GetpixelShaderBlob() { return pixelShaderBlob_; }
};