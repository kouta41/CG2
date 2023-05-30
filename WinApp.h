#pragma once
#include <Windows.h>
#include<string>
#include <cstdint>

class WinApp {
private:
	/*--ウィンドウクラスを登録する---*/
	WNDCLASS wc_;

	//クライアントの領域サイズ
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	//windows Title
	std::wstring title_;
	//ウィンドウサイズを表す構造体
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
};