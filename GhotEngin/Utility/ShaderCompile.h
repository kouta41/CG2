#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

class ShaderCompile {
public: // メンバ関数

	// CompileShader関数
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
};
