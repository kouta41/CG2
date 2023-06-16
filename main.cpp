#include<Windows.h>
#include "WinApp.h"
#include"Directx12.h"
#include"Triangle.h"

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = new WinApp(1280, 720, L"CG2");
	DirectX12* dx12Common = new DirectX12();
	Triangle* triangle = new Triangle();

	//winApp->GetInstance();
	winApp->RegistrateWindowClass();
	//
	winApp->CreateAppWindow();
	//ウィンドウを表示する
	winApp->ShowAppWindow();

	dx12Common->Init();

	dx12Common->Initdxcommand(winApp);

	triangle->Init(dx12Common);

	

	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0) {

		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			dx12Common->Loadcommand(triangle);

			dx12Common->CreateFence();
			
		}
	}
	dx12Common->DirectXRelease(winApp, triangle);
	return 0;
}