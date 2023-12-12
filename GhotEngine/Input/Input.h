#pragma once
#include "Window.h"
#include <cassert>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")

class Input {
public:


	static Input* GetInstance();

	static void Initialize();

	static void Update();

	bool PushKey(uint8_t keyNum);

	bool PushKeyPressed(uint32_t keyNum);

private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE keys[256] = {};
	BYTE preKeys[256] = {};

	bool isInitialize = false;



	//0x80=押している状態
	//0x00=押してない状態
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};