#include"ImguiManager.h"



ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(Window* Win, DirectX12* dxCommon)
{
#ifdef _DEBUG
	dxCommon_ = dxCommon;

	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(Win->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(), static_cast<int>(dxCommon_->GetBufferCount().BufferCount),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dxCommon_->GetSRV(),
		dxCommon_->GetSRV()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon_->GetSRV()->GetGPUDescriptorHandleForHeapStart());
#endif

}

void ImGuiManager::Finalize()
{
#ifdef _DEBUG

	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif
}

void ImGuiManager::Begin()
{
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImGuiManager::End()
{
#ifdef _DEBUG
	ImGui::Render();
#endif
}

void ImGuiManager::Draw()
{
#ifdef _DEBUG

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { DirectX12::GetInstance()->GetSRV() };
	DirectX12::GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12::GetCommandList());
#endif
}
