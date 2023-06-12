#include "WinApp.h"


WinApp::WinApp(int width, int height, std::wstring title) {
	this->kClientWidth_ = width;
	this->kClientHeight_ = height;
	this->title_ = title;

	this->wc_ = {};

	this->wrc_ = { 0,0,this->kClientWidth_ ,this->kClientHeight_ };


}

WinApp::~WinApp() {

}

void WinApp::RegistrateWindowClass() {
	//ウインドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウインドウクラス名（何でもよい）
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウインドウクラスを登録する
	RegisterClass(&wc_);
}

LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


void WinApp::CreateAppWindow() {
	this->hwnd_ = CreateWindow(
		wc_.lpszClassName,        // 利用するクラス名
		L"CG2",					 // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	 // よく見るウィンドウスタイル
		CW_USEDEFAULT,			 // 表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			 // 表示Y座標(WindowsOSに任せる)
		wrc_.right - wrc_.left,	 // ウィンドウ横幅
		wrc_.bottom - wrc_.top,	 // ウィンドウ縦幅
		nullptr,				 // 親ウィンドウハンドル
		nullptr,				 // メニューハンドル
		wc_.hInstance,			 // インスタンスハンドル
		nullptr);				 // オプション
}

void WinApp::ShowAppWindow() {
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinApp::ProcessMessage() {
	MSG msg{}; // メッセージ

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return false;
		}
		
	}
	return true;
}