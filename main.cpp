#include <Windows.h>
#include<cstdint>
#include<format>
#include <string>
#include<d3d12.h>
#include<cassert> 


#include"WinApp.h"
#include"ConvertString.h"
#include"DirectX12.h"


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = new WinApp(1280, 720, L"CG2");
	DirectX12* dxCommon = new DirectX12();
	

	//
	winApp->RegistrateWindowClass();
	//
	winApp->CreateAppWindow();
	//
	winApp->ShowAppWindow();
	//ウィンドウを表示する
	winApp->ShowAppWindow();


	/// <summary>
	/// 
	/// </summary>
	dxCommon->UpdateDirectX();

	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (WinApp::ProcessMessage() == 0) {

		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			/// <summary>
			/// コマンド積む
			/// </summary>
			dxCommon->CommandPack();

		}
	}
}