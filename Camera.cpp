#include "Camera.h"

void Cameraex::Initialize(WinApp* winapp) {
	cameraTransform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -10.0f} };
	winapp_ = winapp;
	/*ImGui::Begin("cameraTransform");
	ImGui::SliderFloat3("vertexData", &cameraTransform.translate.x, -1.0f, 1.0f);
	ImGui::End();*/
}

void Cameraex::Update() {
	worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f });
	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winapp_->GetkClientWidth()) / float(winapp_->GetkClientHeight()), 0.1f, 100.0f);
	worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData = &worldViewProjectionMatrix;
}

void Cameraex::Draw() {
#pragma region ImGui
	ImGui::Begin("Camera");
	float ImGuiScale[Vector3D] = { cameraTransform.scale.x,cameraTransform.scale.y ,cameraTransform.scale.z };
	ImGui::SliderFloat3("Scale", ImGuiScale, 1, 30, "%.3f");
	cameraTransform.scale = { ImGuiScale[x],ImGuiScale[y],ImGuiScale[z] };
	float ImGuiRotate[Vector3D] = { cameraTransform.rotate.x,cameraTransform.rotate.y ,cameraTransform.rotate.z };
	ImGui::SliderFloat3("Rotate", ImGuiRotate, 0, 5, "%.3f");
	cameraTransform.rotate = { ImGuiRotate[x],ImGuiRotate[y],ImGuiRotate[z] };
	float ImGuiTranslate[Vector3D] = { cameraTransform.translate.x,cameraTransform.translate.y ,cameraTransform.translate.z };
	ImGui::SliderFloat3("Translate", ImGuiTranslate, -10, 10, "%.3f");
	cameraTransform.translate = { ImGuiTranslate[x],ImGuiTranslate[y],ImGuiTranslate[z] };
	ImGui::End();
#pragma endregion

}