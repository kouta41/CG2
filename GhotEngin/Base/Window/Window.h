#pragma once
#include <Windows.h>
#include <cstdint>

#include "ImGuiManager.h"

class Window {
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int32_t kWindowWidth = 1280; // 横
	static const int32_t kWindowHeight = 720; // 縦

public: // メンバ関数

	// シングルトンインスタンスの取得
	static Window* GetInstance();


	// ウィンドウプロシージャ
	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// メッセージ処理
	bool ProcessMessage();

	// ウィンドウを作る
	void CreateGameWin(
		const wchar_t* title, UINT WinStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kWindowHeight);

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void TerminateGameWin();

	// ウィンドウハンドルの取得
	HWND GetHwnd() const { return hwnd_; }

	// ウィンドウクラスの取得
	WNDCLASS GetWc() const { return wc; }

private: // メンバ関数
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	const Window& operator=(const Window&) = delete;

private: // メンバ変数

	// Win関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASS wc{}; // ウィンドウクラス
	UINT WinStyle_;

};