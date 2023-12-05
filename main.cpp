#include "GhotEngin.h"

// Winsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GhotEngin* ghotEngin = new GhotEngin;
	ghotEngin->Initialize();
	ghotEngin->Update();
	ghotEngin->Finalize();
	delete ghotEngin;
	return 0;
}