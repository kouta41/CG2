#include "Camera.h"

void Camera::Initialize() {
	cameraTransform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -10.0f} };
}

void Camera::Update() {
	worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f });
	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winapp_->GetkClientWidth()) / float(winapp_->GetkClientHeight()), 0.1f, 100.0f);
	worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData = &worldViewProjectionMatrix;
}

void Camera::Draw() {}