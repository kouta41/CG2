#include <Windows.h>


#include"WinApp.h"


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = new WinApp(1280, 720, L"CG2");
	
	
	//
	winApp->RegistrateWindowClass();
	//
	winApp->CreateAppWindow();
	//
	winApp->ShowAppWindow();
	//ウィンドウを表示する
	winApp->ShowAppWindow();



	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0) {

		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			

		}
	}
}