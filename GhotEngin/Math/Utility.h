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


#include "ConvertString.h"
#include "externals/DirectXTex/DirectXTex.h"



struct Vector2 final {
	float x;
	float y;
};

struct Vector3 final {
	float x;
	float y;
	float z;
};
enum NumberVector3 {
	x,
	y,
	z,
	Vector3D,
};
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
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

struct Matrix4x4 final {
	float m[4][4];
};
struct Matrix3x3 final {
	float m[3][3];
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

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float rotate);
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float rotate);
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float rotate);
// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//平行投射行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip);
//逆行(列
Matrix4x4 Inverse(const Matrix4x4& m);

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