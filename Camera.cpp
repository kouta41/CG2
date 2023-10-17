#include"Camera.h"

void Camera::Initialize(int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;
}

void Camera::Update()
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;
}

void Camera::ImGui()
{
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