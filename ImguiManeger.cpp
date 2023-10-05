#include "ImguiManege.h"


void ImGuiManeger::Initialize(WinApp* winApp_, DirectXCommon* dir_) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->Gethwnd());
	ImGui_ImplDX12_Init(dir_->GetDevice(),
		dir_->swapChainDesc.BufferCount,
		dir_->rtvDesc.Format,
		dir_->srvDescriptorHeap_,
		dir_->srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
		dir_->srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart()
	);
}

void ImGuiManeger::Update() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManeger::Draw(DirectXCommon* dir_) {
	// 開発用UIの処理。実際に開発用のUIを出す場合はココをゲーム固有の処理に置き換える
	//ImGui::ShowDemoWindow();
	// ImGuiの内部コマンドを生成する
	ImGui::Render();
	// 実際にcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dir_->commandList_);
}

void ImGuiManeger::Release() {
	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}