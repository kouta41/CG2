#pragma once
#include <wrl.h>
#include <Windows.h>

using namespace Microsoft::WRL;
//入力
class Input {

public://メンバ関数
	//初期化
	void Init(HINSTANCE hInstance,HWND hwnd);

	//更新
	void Update();
};