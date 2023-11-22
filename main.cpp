#include "GhotEngin.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GhotEngin* ghotEngin = new GhotEngin;
	ghotEngin->Initialize();
	ghotEngin->Update();
	ghotEngin->Finalize();
	delete ghotEngin;
	return 0;
}