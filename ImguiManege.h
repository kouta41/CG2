#pragma once

#include "DirectX12.h"
#include "WinApp.h"
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

#include <Windows.h>
#include <cstdint>
#include <d3d12.h>

class WinApp;
class DirectXCommon;

class ImGuiManeger {
public:
	void Initialize(WinApp* winApp_, DirectXCommon* dir_);

	void Update();

	void Draw(DirectXCommon* dir_);

	void Release();
};

