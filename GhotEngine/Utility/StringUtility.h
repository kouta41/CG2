#pragma once
#include <string>
#include <Windows.h>

// string->wstring
std::wstring ConvertString(const std::string& str);
// wstring->string
std::string  ConvertString(const std::wstring& str);

void Log(const std::string& message);
void Log(const std::wstring& message);