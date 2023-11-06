#include <Windows.h>

#include "WinApp.h"
#include "Utility.h"
#include "DirectX12.h"
#include "Mesh.h"
#include "Triangle.h"
#include "ImguiManege.h"
#include "Camera.h"

// WIndowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	CoInitializeEx(0, COINIT_MULTITHREADED);

	// 三角形の数
	const int Max = 30;

	Vector4 pos[Max][3];
	for (int i = 1; i < Max; i++) {
		// 左下
		pos[i][0] = { -0.95f - (i * -0.07f),0.0f,0.0f,1.0f };
		// 上
		pos[i][1] = { -0.92f - (i * -0.07f),0.05f,0.0f,1.0f };
		// 右下
		pos[i][2] = { -0.89f - (i * -0.07f),0.0f,0.0f,1.0f };
	}

	// 左下
	pos[0][0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	// 上
	pos[0][1] = { 0.0f, 0.5f, 0.0f, 1.0f };
	// 右下
	pos[0][2] = { 0.5f, -0.5f, 0.0f, 1.0f };


	// 左下2
	pos[1][0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	// 上2
	pos[1][1] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// 右下2
	pos[1][2] = { 0.5f, -0.5f, 0.0f, 1.0f };


	WinApp* winapp = new WinApp(L"CG2");
	DirectXCommon* directX = new DirectXCommon();
	Mesh* mesh = new Mesh();
	Triangle* triangle[Max];
	ImGuiManeger* imgui = new ImGuiManeger();
	Camera* camera = new Camera();
	directX->Initialize(winapp);
	mesh->Initialize(directX,winapp);

	camera->Initialize();

	MSG msg{};

	for (int i = 0; i < Max; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(directX, pos[i],winapp);
	}

	Transform transform{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	Transform cameraTransform{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -5.0f} };
	imgui->Initialize(winapp, directX);

	// ウインドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ゲームの処理
			camera->Update();
			imgui->Update();
			directX->Update();
			mesh->Update(directX);
			
			

			//transform.rotate.y += 0.03f;

			
			for (int i = 1; i < Max; i++) {
			//	*triangle[i]->GetwvpData() = *camera->GettransformationMatrixData();
				//triangle[i]->Draw(directX);
			}

			//triangle[0]->Draw(directX);

			//triangle[0]->DrawSprite(directX);

			triangle[0]->DrawSphere(directX);


			//triangle[0]->DrawOBJ(directX, *camera->GettransformationMatrixData());
			

			ImGui::Begin("vertexData");
			ImGui::SliderFloat3("vertexData", &triangle[0]->transform_.translate.x, -1.0f, 1.0f);
			ImGui::SliderFloat3("vertexData1", &triangle[1]->transform_.translate.x, -1.0f, 1.0f);

			ImGui::End();

			/*ImGui::Begin("Mesh Color");
			ImGui::ColorEdit3("Mesh Color", &triangle[0]->GetmaterialData()->x);
			ImGui::End();

			ImGui::Begin("Mesh Position");
			ImGui::SliderFloat3("Mesh Pos", &transform.translate.x, -1.0f, 1.0f);
			ImGui::End();

			ImGui::Begin("Camera Position");
			ImGui::SliderFloat3("Camera Pos", &cameraTransform.translate.x, -1.0f, 1.0f);
			ImGui::SliderFloat3("Camera scale", &cameraTransform.scale.x, -1.0f, 1.0f);
			ImGui::SliderFloat3("Camera rotate", &cameraTransform.rotate.x, -1.0f, 1.0f);
			ImGui::End();*/


			imgui->Draw(directX);

			directX->Close();
		}
		CoUninitialize();
	}

	for (int i = 0; i < Max; i++) {
		triangle[i]->Release();
		delete triangle[i];
	}

	
	imgui->Release();
	directX->Release(winapp);
	mesh->Release();

	delete camera;
	delete mesh;
	delete imgui;
	delete directX;

	return 0;
}