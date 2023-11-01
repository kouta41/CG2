#pragma once
#include "Utility.h"
#include "WinApp.h"
#include "imgui.h"

class Camera {

public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize();

	/// <summary>
	/// 
	/// </summary>
	void Update();

	void Draw();

	Matrix4x4* GettransformationMatrixData() { return transformationMatrixData; }
public:

	WinApp* winapp_ = nullptr;

	Transform cameraTransform;
	Matrix4x4 worldMatrix;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4* transformationMatrixData;

};