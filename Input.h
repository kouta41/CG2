#pragma once
#include <wrl.h>
#include <Windows.h>
#include <cassert>

#define DIRECTINPUT_VERSION   0x0800 //DirectInputのバージョン指定
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace Microsoft::WRL;
//入力
class Input {
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	//初期化
	void Init(HINSTANCE hInstance,HWND hwnd);

	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

private://メンバ関数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};


	ComPtr <IDirectInput8> directInput = nullptr;


};