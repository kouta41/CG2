#pragma once
#include <string>
#include <d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#include"ConvertString.h"
#include<format>
#include"WinApp.h"

#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectX12 {
public:
	static DirectX12* GetInstance();
	void Initirize();
	void Update();
	void Draw();

private:
	IDXGIFactory* dxgiFactory;
	IDXGIAdapter4* useAdapte;
	ID3D12Device* device;
};

