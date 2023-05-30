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
	

	WinApp* winApp = new WinApp(1280,720,L"CG2");


	DirectX12* dxCommon = nullptr;
	dxCommon = DirectX12::GetInstance();
	dxCommon->Initirize();


	//ウィンドウを表示する
	winApp->ShowAppWindow();
	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (winApp->ProcessMessage==0) {
		//windowsにメッセージが来たら最優先で処理させる
		return 0;
		}
		

			//ゲーム処理
			// 

	//出力ウインドウへの文字出力
			OutputDebugStringA("Hello,DirectX!\n");
		}
	}
	return 0;
}