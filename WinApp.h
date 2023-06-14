#pragma once
#include <Windows.h>
#include<string>

class WinApp {

private:
	/*--ウィンドウクラスを登録する---*/
	WNDCLASS wc_;

	//クライアント領域サイズ
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	//windows Title
	std::wstring title_;
	//ウィンドウサイズを表す構造佁E
	RECT wrc_;

	//ウィンドウ
	HWND hwnd_;

public:

	WinApp(
		int width,
		int height,
		std::wstring title
	);

	~WinApp();

	//ウィンドウクラスを登録
	void RegistrateWindowClass();

	static LRESULT WindowProc(HWND hwnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

	//ウィンドウの生成
	void CreateAppWindow();


	//ウィンドウを表示する
	void ShowAppWindow();

	//メッセージの処理
	bool ProcessMessage();

	//int GetkClientWidth_() { return kClientWidth_; }
	//int GetkClientHeight_() { return kClientHeight_; }


	//
	HWND Gethwnd_() { return this->hwnd_; }

};