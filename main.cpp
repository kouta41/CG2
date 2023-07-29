#include<Windows.h>
#include "WinApp.h"
#include"Directx12.h"
#include"Triangle.h"
#include"ImguiManege.h"

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	int count = 10;
	WinApp* winApp = new WinApp(1280, 720, L"CG2");
	DirectX12* dx12Common = new DirectX12();
	Triangle* triangle[10] = { new Triangle() };
	Imgui* imgui = new Imgui();


	//winApp->GetInstance();
	winApp->RegistrateWindowClass();
	//
	winApp->CreateAppWindow();
	//ウィンドウを表示する
	winApp->ShowAppWindow();

	dx12Common->Init(winApp);


	

	for (int i = 0; i < count; i++) {
		triangle[i] = new Triangle();
		triangle[i]->triangleData[0] = { -0.8f + (i * 0.1f),0.8f,0.0f,1.0f };
		triangle[i]->triangleData[1] = { -0.7f + (i * 0.1f),0.9f,0.0f,1.0f };
		triangle[i]->triangleData[2] = { -0.6f + (i * 0.1f),0.8f,0.0f,1.0f };
		triangle[i]->Init(dx12Common);
	}	

	imgui->Init(dx12Common, winApp);
	

	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0) {


		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの更新処理

			dx12Common->Update();
			imgui->Update();

			for (int i = 0; i < count; i++)
			{
				triangle[i]->Draw(triangle[i]->triangleData);
			}
			imgui->Draw(dx12Common);
			dx12Common->Draw();
		}
	}
		dx12Common->Release();
		for (int i = 0; i < count; i++)
		{
			triangle[i]->Release();
		}
	return 0;
}