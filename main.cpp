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
	

	WinApp* winapp = new WinApp();

	winapp->Initirize(L"CG2");

	DirectX12* dxCommon = nullptr;
	dxCommon = DirectX12::GetInstance();
	dxCommon->Initirize();


	MSG msg{};
	//ウインドウのXボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//windowsにメッセージが来たら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			//ゲーム処理
			// 

	//出力ウインドウへの文字出力
			OutputDebugStringA("Hello,DirectX!\n");
		}
	}
	return 0;
}