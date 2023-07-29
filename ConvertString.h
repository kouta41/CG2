#pragma once
#include <string>
#include <Windows.h>
#include <dxcapi.h>
#include <format>
#include <debugapi.h>
#include <cassert>
#pragma comment(lib,"dxcompiler.lib")

std::string ConvertString(const std::wstring& str);
std::wstring ConvertString(const std::string& str);

void Log(const std::string& message);
