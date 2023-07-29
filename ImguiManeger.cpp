#include"ImguiManege.h"
#include "ConvertString.h"


Imgui::Imgui() {
	
	commandList_ = nullptr;
}
void Imgui::Init(DirectX12* dx12Common, WinApp* winApp) {
	//ImGuiの初期化。詳細はさして重要ではないので解説は省略する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->Gethwnd_());
	ImGui_ImplDX12_Init(dx12Common->Getdevice(),
		dx12Common->GetswapChainDesc().BufferCount,
		dx12Common->GetrtvDesc().Format,
		dx12Common->GetsrvDescriptorHeap_(),
		dx12Common->GetsrvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart(),
		dx12Common->GetsrvDescriptorHeap_()->GetGPUDescriptorHandleForHeapStart()
	);

	commandList_ = dx12Common->GetcommandList();

}

void Imgui::Update() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::ShowDemoWindow();
}


void Imgui::Draw(DirectX12* dx12Common) {

	
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { dx12Common->GetsrvDescriptorHeap_() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);

	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_);
}