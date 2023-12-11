#include "GhotEngin.h"

// Winsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Engine> engine;
	engine = std::make_unique<Engine>();
	engine->Initialize();
	engine->Run();
	engine->Finalize();
}