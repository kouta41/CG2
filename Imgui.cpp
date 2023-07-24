#include"Imgui.h"
#include "ConvertString.h"


Imgui::Imgui() {
	rtvDescriptorHeap = nullptr;
	srvDescriptorHeap = nullptr;
	device_=nullptr;
}

Imgui::~Imgui() {
	//RTV用のヒープでディスクリプタのかずは２。RTVはShader内で触るものがないのでShaderVisibleはfalse
	rtvDescriptorHeap = CreatDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	//SRV用のヒープでディスクリプタの数は１２８。SRVはShader内で触るものなのでShaderVisibleはtrue
	srvDescriptorHeap = CreatDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	if (Imgui_ImplWin32_WndProcHandler()) {
		return true;
	}
}

void Imgui::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark(hwnd);

}

void Imgui::Update() {

}

void Imgui::Draw() {

}