#pragma once
#include "DirectX12.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

struct Resource {
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
};

struct VertexData final {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material final {
	Vector4 color;
	int32_t enableLighting;
};

struct DirectionalLight final{
	Vector4 color; //ライトの色
	Vector3 direction; //ライトの向き
	float intensity; //ライトの輝度
};

class CreateResource{
public:
	//resource
	static Microsoft::WRL::ComPtr<ID3D12Resource>CreateBufferResource(size_t sizeInBytes);
	// IBV
	static D3D12_INDEX_BUFFER_VIEW CreateIndexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes);
	// VBV
	static D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes, int size);
};