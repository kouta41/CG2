#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <sstream>
#include "WinApp.h"
#include "DirectX12.h"
#include "Camera.h"
#include "ImguiManege.h"
#include "Utility.h"
#include "Triangle.h"

class Model
{
public:


	void Initialize(DirectXCommon* dir_,Triangle* tri_);

	void Update(const Matrix4x4& transformationMatrixData);

	void Draw(DirectXCommon* dir_, Triangle* tri_);

	void Release();

	void CreateVertexResource(DirectXCommon* dir_, Triangle* tri_);
	void CreateMaterialResource(DirectXCommon* dir_, Triangle* tri_);
	void CreateWVPResource(DirectXCommon* dir_, Triangle* tri_);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
private:

	static WinApp* winapp_;

	ModelData modelData;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;

	VertexData* vertexData;
	Material* materialData;
	TransformationMatrix* wvpData;

	Transform transform;
	Transform uvTransform;

	bool isModel;
};

