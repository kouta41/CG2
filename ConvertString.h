#pragma once
#include <string>
#include <Windows.h>


std::string ConvertString(const std::wstring& str);
std::wstring ConvertString(const std::string& str);



class Function {
public:

	static void Log(const std::string& message);

};

#include<Windows.h>
#include<dxcapi.h>
#include<string>
#include<format>
#include <debugapi.h>
#include "ConvertString.h"
#include <cassert>


#pragma comment(lib,"dxcompiler.lib")

IDxcBlob* CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するprofile
	const wchar_t* profile,
	//初期化で生成したものを３つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);