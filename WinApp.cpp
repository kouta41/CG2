#include "WinApp.h"

//ウインドウプロシージャ
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
//	WPARAM wparam, LPARAM lparam) {
//	//メッセージに応じてゲーム固有の処理を行う
//	switch (msg) {
//		//ウインドウが破棄された
//	case WM_DESTROY:
//		//OSに対して、アプリの終了を伝える
//		PostQuitMessage(0);
//		return 0;
//	}
//	//標準のメッセージ処理を行う
//	return DefWindowProc(hwnd, msg, wparam, lparam);
//}

class Win
{
public:
	Win();
	~Win();

private:
	Win Draw() {}
};
