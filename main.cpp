#include<Windows.h>
#include "WinApp.h"
#include"Directx12.h"
#include"Triangle.h"
#include"Vector4.h"


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	int kaunt = 10;
	WinApp* winApp = new WinApp(1280, 720, L"CG2");
	DirectX12* dx12Common = new DirectX12();
	Triangle* triangle[10] = { new Triangle() };

	//winApp->GetInstance();
	winApp->RegistrateWindowClass();
	//
	winApp->CreateAppWindow();
	//ウィンドウを表示する
	winApp->ShowAppWindow();

	dx12Common->Init();

	dx12Common->Initdxcommand(winApp);


	//triangle[0]->triangleData[0] = { -0.8f,0.8f,0.0f,1.0f };

	/*triangle[0]->triangleData[1] = { -0.7f,0.9f,0.0f,1.0f };

	triangle[0]->triangleData[2] = { -0.6f,0.8f,0.0f,1.0f };


	triangle[0]->triangleData[0] = { -0.7f,0.8f,0.0f,1.0f };

	triangle[0]->triangleData[1] = { -0.6f,0.9f,0.0f,1.0f };

	triangle[0]->triangleData[2] = { -0.5f,0.8f,0.0f,1.0f }*/;
	                                              
	for (int i = 0; i < kaunt; i++) {
		triangle[i] = new Triangle();

		triangle[i]->triangleData[0] = { -0.8f + (i * 0.1f),0.8f,0.0f,1.0f };

		triangle[i]->triangleData[1] = { -0.7f + (i * 0.1f),0.9f,0.0f,1.0f };

		triangle[i]->triangleData[2] = { -0.6f + (i * 0.1f),0.8f,0.0f,1.0f };
	}


	//手順

	// 0. Kamata Engineを参考にしてVector4.hをつくる X
	// 1. 三角形をひとつ描画する X
	// 2. 三角形のデータの構造体を作る X
	// 3. 構造体化された三角形をひとつ描画する X
	// 4. 複数個の三角形を描画する
	

	for (int i = 0; i < kaunt; i++) {
		triangle[i]->Init(dx12Common, triangle[i]->triangleData);
	}
	

	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0) {

		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			dx12Common->Loadcommand();

			for (int i = 0; i < kaunt; i++)
			{
				triangle[i]->Loadcommand(dx12Common);
			}

			dx12Common->CreateFence();	
		}
	}
		dx12Common->DirectXRelease(winApp);
		for (int i = 0; i < kaunt; i++)
		{
			triangle[i]->TriangleRelease();
		}
	return 0;
}