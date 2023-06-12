#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp {
private:
	/*--ウィンドウクラスを登録する---*/
	WNDCLASS wc_;

	//クライアント�E領域サイズ
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

	//ウィンドウの生�E
	void CreateAppWindow();
	

	//ウィンドウを表示する
	void ShowAppWindow();

	//メチE��ージの処琁E
	bool ProcessMessage();
};