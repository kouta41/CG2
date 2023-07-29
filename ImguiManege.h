#pragma once
#include <Windows.h>
#include <dxgi1_6.h>

#include "DirectX12.h"
#include "WinApp.h"
#include "Utility.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

class Imgui {
private:
	ID3D12GraphicsCommandList* commandList_;


public:
	Imgui();

	void Init(DirectX12* dx12Common,WinApp* winApp);

	void Update();

	void Draw(DirectX12* dx12Common);

};