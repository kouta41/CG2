#include"ImguiManege.h"
#include "ConvertString.h"


Imgui::Imgui() {
	rtvDescriptorHeap = nullptr;
	srvDescriptorHeap = nullptr;
	commandList_ = nullptr;
	device_ = nullptr;
	hwnd;
	swapChainDesc;
	rtvDesc;
}
void Imgui::Init() {
	//RTV用のヒープでディスクリプタのかずは２。RTVはShader内で触るものがないのでShaderVisibleはfalse
	rtvDescriptorHeap = CreatDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	//SRV用のヒープでディスクリプタの数は１２８。SRVはShader内で触るものなのでShaderVisibleはtrue
	srvDescriptorHeap = CreatDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	//ImGuiの初期化。詳細はさして重要ではないので解説は省略する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(device_,
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::ShowDemoWindow();
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
}

void Imgui::Draw() {
	ID3D12CommandList* commandLists[] = { commandList_ };
	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);

	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_);
}