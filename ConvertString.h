#pragma once
#include <string>
#include <Windows.h>
#include <dxcapi.h>
#include <format>
#include <debugapi.h>
#include <cassert>
#include "ConvertString.h"
#pragma comment(lib,"dxcompiler.lib")

std::string ConvertString(const std::wstring& str);
std::wstring ConvertString(const std::string& str);


class Function {
public:

	static void Log(const std::string& message);

};



IDxcBlob* CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するprofile
	const wchar_t* profile,
	//初期化で生成したものを３つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);


struct Vector3 final {
	float x;
	float y;
	float z;
};

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct Matrix4x4 {
	float m[4][4];
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
//逆行(列
Matrix4x4 Inverse(const Matrix4x4& m);

//
