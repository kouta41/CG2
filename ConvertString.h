#pragma once
#include <string>
#include <Windows.h>


std::string ConvertString(const std::wstring& str);
std::wstring ConvertString(const std::string& str);
