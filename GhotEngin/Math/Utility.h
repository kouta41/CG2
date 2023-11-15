#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <cassert>
#include <format>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cstdint>
#include <string>
#include <format>

#include"Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "ConvertString.h"
//#include "externals/DirectXTex/DirectXTex.h"





enum NumberVector3 {
	x,
	y,
	z,
	Vector3D,
};




struct VertexData final {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};


struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};



struct Sphere final {
	Vector3 center;//中心点
	float radius;//半径
};



struct Material final{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct DirectionalLight
{
	Vector4 color; //ライトの色
	Vector3 direction; //ライトの向き
	float intensity; //ライトの輝度
};
struct MaterialData
{
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	int TextureIndex;
};

Matrix4x4 MakeIdentity4x4();



void Log(const std::string& message);

 IDxcBlob* CompileShader(
	// CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	// Compilerに使用するProfile
	const wchar_t* profile,
	// 初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);


 D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

 D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);