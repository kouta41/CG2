#pragma once
#include <format>

#include "Matrix4x4.h"
#include "Vector3.h"

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float rotate);

// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float rotate);

// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float rotate);

// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//平行投射行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip);

//逆行(列
Matrix4x4 Inverse(const Matrix4x4& m);

// 単位行列
Matrix4x4 MakeIdentityMatrix();

