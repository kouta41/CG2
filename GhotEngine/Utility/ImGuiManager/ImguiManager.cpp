#include "ImGuiManager.h"

#include "DirectX12.h"
#include "Window.h"
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

ImGuiManager* ImGuiManager::GetInstance() {
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(), static_cast<int>(dxCommon_->GetBufferCount().BufferCount),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dxCommon_->GetSRV(),
		dxCommon_->GetSRV()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon_->GetSRV()->GetGPUDescriptorHandleForHeapStart());

}

void ImGuiManager::Finalize() {

	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


}

void ImGuiManager::Begin() {
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End() {
	ImGui::Render();
}

void ImGuiManager::Draw() {

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSRV() };
	DirectXCommon::GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetCommandList());

}