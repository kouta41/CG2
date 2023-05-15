#include <Windows.h>
#include<cstdint>
#include<format>
#include <string>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert> 
#include<dxgidebug.h>
#include<dxcapi.h>

#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include"WinApp.h"


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	

	WinApp* winapp = new WinApp(L"CG2");

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