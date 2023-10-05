#pragma once
#include <Windows.h>
#include <dxgi1_6.h>

#include "DirectX12.h"
#include "WinApp.h"

class Imgui {
private:

public:

	void Init(DirectX12* dx12Common,WinApp* winApp);

	void Update();

	void Draw();

};