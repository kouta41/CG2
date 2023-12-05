#include "Window.h"
#include <string>
#include <imgui_impl_win32.cpp>

Window* Window::GetInstance() {
	static Window instance;
	return &instance;
}

// ウィンドウプロシージャ
LRESULT Window::WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された	
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}


	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// メッセージ処理
bool Window::ProcessMessage() {
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}

// ゲームウィンドウ
void Window::CreateGameWin(
	const wchar_t* title, UINT WinStyle, int32_t clientWidth, int32_t clientHeight) {

	// ウィンドウクラスの設定
	wc.lpfnWndProc = (WNDPROC)WinProc;     // ウィンドウプロシージャ
	wc.lpszClassName = title;      // ウィンドウクラス名
	wc.hInstance = GetModuleHandle(nullptr);  // ウィンドウハンドル
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	RegisterClass(&wc); // ウィンドウクラスをOSに登録

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&wrc, WinStyle_, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(
		wc.lpszClassName, // クラス名
		title,                   // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,            // タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,           // 表示X座標（OSに任せる）
		CW_USEDEFAULT,           // 表示Y座標（OSに任せる）
		wrc.right - wrc.left,    // ウィンドウ横幅
		wrc.bottom - wrc.top,    // ウィンドウ縦幅
		nullptr,                 // 親ウィンドウハンドル
		nullptr,                 // メニューハンドル
		wc.hInstance,     // 呼び出しアプリケーションハンドル
		nullptr);                // オプション

	// ウィンドウ表示
	ShowWindow(hwnd_, SW_SHOW);
}

void Window::TerminateGameWin() {

	// ウィンドウクラスを登録解除
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	// COM 終了
	CoUninitialize();
}