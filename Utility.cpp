#include "Utility.h"

ID3D12DescriptorHeap* CreatDescriptorHeap(
	ID3D12Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors,
	bool shaderVisible) {
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}


Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 MakeIdentity4x4;
	MakeIdentity4x4.m[0][0] = 1;
	MakeIdentity4x4.m[0][1] = 0;
	MakeIdentity4x4.m[0][2] = 0;
	MakeIdentity4x4.m[0][3] = 0;

	MakeIdentity4x4.m[1][0] = 0;
	MakeIdentity4x4.m[1][1] = 1;
	MakeIdentity4x4.m[1][2] = 0;
	MakeIdentity4x4.m[1][3] = 0;

	MakeIdentity4x4.m[2][0] = 0;
	MakeIdentity4x4.m[2][1] = 0;
	MakeIdentity4x4.m[2][2] = 1;
	MakeIdentity4x4.m[2][3] = 0;

	MakeIdentity4x4.m[3][0] = 0;
	MakeIdentity4x4.m[3][1] = 0;
	MakeIdentity4x4.m[3][2] = 0;
	MakeIdentity4x4.m[3][3] = 1;

	return 	MakeIdentity4x4;

}

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] };
	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 MakeScaleMatrix;
	MakeScaleMatrix.m[0][0] = scale.x;
	MakeScaleMatrix.m[0][1] = 0.0f;
	MakeScaleMatrix.m[0][2] = 0.0f;
	MakeScaleMatrix.m[0][3] = 0.0f;

	MakeScaleMatrix.m[1][0] = 0.0f;
	MakeScaleMatrix.m[1][1] = scale.y;
	MakeScaleMatrix.m[1][2] = 0.0f;
	MakeScaleMatrix.m[1][3] = 0.0f;

	MakeScaleMatrix.m[2][0] = 0.0f;
	MakeScaleMatrix.m[2][1] = 0.0f;
	MakeScaleMatrix.m[2][2] = scale.z;
	MakeScaleMatrix.m[2][3] = 0.0f;

	MakeScaleMatrix.m[3][0] = 0.0f;
	MakeScaleMatrix.m[3][1] = 0.0f;
	MakeScaleMatrix.m[3][2] = 0.0f;
	MakeScaleMatrix.m[3][3] = 1.0f;

	return MakeScaleMatrix;
}
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float rotate) {
	Matrix4x4 MakeRotateXMatrix;
	MakeRotateXMatrix.m[0][0] = 1;
	MakeRotateXMatrix.m[0][1] = 0;
	MakeRotateXMatrix.m[0][2] = 0;
	MakeRotateXMatrix.m[0][3] = 0;

	MakeRotateXMatrix.m[1][0] = 0;
	MakeRotateXMatrix.m[1][1] = std::cos(rotate);
	MakeRotateXMatrix.m[1][2] = std::sin(rotate);
	MakeRotateXMatrix.m[1][3] = 0;

	MakeRotateXMatrix.m[2][0] = 0;
	MakeRotateXMatrix.m[2][1] = -std::sin(rotate);
	MakeRotateXMatrix.m[2][2] = std::cos(rotate);
	MakeRotateXMatrix.m[2][3] = 0;

	MakeRotateXMatrix.m[3][0] = 0;
	MakeRotateXMatrix.m[3][1] = 0;
	MakeRotateXMatrix.m[3][2] = 0;
	MakeRotateXMatrix.m[3][3] = 1;

	return MakeRotateXMatrix;
}
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float rotate) {
	Matrix4x4 MakeRotateYMatrix;
	MakeRotateYMatrix.m[0][0] = std::cos(rotate);
	MakeRotateYMatrix.m[0][1] = 0;
	MakeRotateYMatrix.m[0][2] = -std::sin(rotate);
	MakeRotateYMatrix.m[0][3] = 0;

	MakeRotateYMatrix.m[1][0] = 0;
	MakeRotateYMatrix.m[1][1] = 1;
	MakeRotateYMatrix.m[1][2] = 0;
	MakeRotateYMatrix.m[1][3] = 0;

	MakeRotateYMatrix.m[2][0] = std::sin(rotate);
	MakeRotateYMatrix.m[2][1] = 0;
	MakeRotateYMatrix.m[2][2] = std::cos(rotate);
	MakeRotateYMatrix.m[2][3] = 0;

	MakeRotateYMatrix.m[3][0] = 0;
	MakeRotateYMatrix.m[3][1] = 0;
	MakeRotateYMatrix.m[3][2] = 0;
	MakeRotateYMatrix.m[3][3] = 1;

	return MakeRotateYMatrix;
}
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float rotate) {
	Matrix4x4 MakeRotateZMatrix;
	MakeRotateZMatrix.m[0][0] = std::cos(rotate);
	MakeRotateZMatrix.m[0][1] = std::sin(rotate);
	MakeRotateZMatrix.m[0][2] = 0;
	MakeRotateZMatrix.m[0][3] = 0;

	MakeRotateZMatrix.m[1][0] = -std::sin(rotate);
	MakeRotateZMatrix.m[1][1] = std::cos(rotate);
	MakeRotateZMatrix.m[1][2] = 0;
	MakeRotateZMatrix.m[1][3] = 0;

	MakeRotateZMatrix.m[2][0] = 0;
	MakeRotateZMatrix.m[2][1] = 0;
	MakeRotateZMatrix.m[2][2] = 1;
	MakeRotateZMatrix.m[2][3] = 0;

	MakeRotateZMatrix.m[3][0] = 0;
	MakeRotateZMatrix.m[3][1] = 0;
	MakeRotateZMatrix.m[3][2] = 0;
	MakeRotateZMatrix.m[3][3] = 1;

	return MakeRotateZMatrix;
}
// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 m4;
	m4.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
		m1.m[0][3] * m2.m[3][0];
	m4.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
		m1.m[0][3] * m2.m[3][1];
	m4.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
		m1.m[0][3] * m2.m[3][2];
	m4.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
		m1.m[0][3] * m2.m[3][3];

	m4.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
		m1.m[1][3] * m2.m[3][0];
	m4.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
		m1.m[1][3] * m2.m[3][1];
	m4.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
		m1.m[1][3] * m2.m[3][2];
	m4.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
		m1.m[1][3] * m2.m[3][3];

	m4.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
		m1.m[2][3] * m2.m[3][0];
	m4.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
		m1.m[2][3] * m2.m[3][1];
	m4.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
		m1.m[2][3] * m2.m[3][2];
	m4.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
		m1.m[2][3] * m2.m[3][3];

	m4.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
		m1.m[3][3] * m2.m[3][0];
	m4.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
		m1.m[3][3] * m2.m[3][1];
	m4.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
		m1.m[3][3] * m2.m[3][2];
	m4.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
		m1.m[3][3] * m2.m[3][3];
	return m4;
}
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 MakeTranslateMatrix;
	MakeTranslateMatrix.m[0][0] = 1.0f;
	MakeTranslateMatrix.m[0][1] = 0.0f;
	MakeTranslateMatrix.m[0][2] = 0.0f;
	MakeTranslateMatrix.m[0][3] = 0.0f;

	MakeTranslateMatrix.m[1][0] = 0.0f;
	MakeTranslateMatrix.m[1][1] = 1.0f;
	MakeTranslateMatrix.m[1][2] = 0.0f;
	MakeTranslateMatrix.m[1][3] = 0.0f;

	MakeTranslateMatrix.m[2][0] = 0.0f;
	MakeTranslateMatrix.m[2][1] = 0.0f;
	MakeTranslateMatrix.m[2][2] = 1.0f;
	MakeTranslateMatrix.m[2][3] = 0.0f;

	MakeTranslateMatrix.m[3][0] = translate.x;
	MakeTranslateMatrix.m[3][1] = translate.y;
	MakeTranslateMatrix.m[3][2] = translate.z;
	MakeTranslateMatrix.m[3][3] = 1.0f;

	return MakeTranslateMatrix;
}
//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 MakeAffineMatrix;

	MakeRotateXMatrix(rotate.x);
	MakeRotateYMatrix(rotate.y);
	MakeRotateZMatrix(rotate.z);

	Matrix4x4 XYZ = Multiply(MakeRotateXMatrix(rotate.x),Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	MakeAffineMatrix.m[0][0] = XYZ.m[0][0] * scale.x;
	MakeAffineMatrix.m[0][1] = XYZ.m[0][1] * scale.x;
	MakeAffineMatrix.m[0][2] = XYZ.m[0][2] * scale.x;
	MakeAffineMatrix.m[0][3] = 0;

	MakeAffineMatrix.m[1][0] = XYZ.m[1][0] * scale.y;
	MakeAffineMatrix.m[1][1] = XYZ.m[1][1] * scale.y;
	MakeAffineMatrix.m[1][2] = XYZ.m[1][2] * scale.y;
	MakeAffineMatrix.m[1][3] = 0;

	MakeAffineMatrix.m[2][0] = XYZ.m[2][0] * scale.z;
	MakeAffineMatrix.m[2][1] = XYZ.m[2][1] * scale.z;
	MakeAffineMatrix.m[2][2] = XYZ.m[2][2] * scale.z;
	MakeAffineMatrix.m[2][3] = 0;

	MakeAffineMatrix.m[3][0] = translate.x;
	MakeAffineMatrix.m[3][1] = translate.y;
	MakeAffineMatrix.m[3][2] = translate.z;
	MakeAffineMatrix.m[3][3] = 1;

	return MakeAffineMatrix;
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 MakePerspectiveFovMatrix;
	MakePerspectiveFovMatrix.m[0][0] = 1/ aspectRatio * (1 / std::tan(fovY / 2));
	MakePerspectiveFovMatrix.m[0][1] = 0.0f;
	MakePerspectiveFovMatrix.m[0][2] = 0.0f;
	MakePerspectiveFovMatrix.m[0][3] = 0.0f;

	MakePerspectiveFovMatrix.m[1][0] = 0.0f;
	MakePerspectiveFovMatrix.m[1][1] = 1 / std::tan(fovY / 2);
	MakePerspectiveFovMatrix.m[1][2] = 0.0f;
	MakePerspectiveFovMatrix.m[1][3] = 0.0f;

	MakePerspectiveFovMatrix.m[2][0] = 0.0f;
	MakePerspectiveFovMatrix.m[2][1] = 0.0f;
	MakePerspectiveFovMatrix.m[2][2] = farClip / (farClip - nearClip);
	MakePerspectiveFovMatrix.m[2][3] = 1.0f;

	MakePerspectiveFovMatrix.m[3][0] = 0.0f;
	MakePerspectiveFovMatrix.m[3][1] = 0.0f;
	MakePerspectiveFovMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	MakePerspectiveFovMatrix.m[3][3] = 0.0f;

	return MakePerspectiveFovMatrix;
}

//2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip){
	Matrix4x4 result;
	float dx = right - left;
	float dy = top - bottom;
	float dz = farCcip - nearClip;
	result = {
		2.0f / dx,0.0f,0.0f,0.0f,
		0.0f,2.0f / dy,0.0f,0.0f,
		0.0f,0.0f,-2.0f / dz,0.0f,
		-((right + left) / dx),-((top + bottom) / dy),-((farCcip + nearClip) / dz),1.0f
	};
	return result;
}
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	float m4s;
	Matrix4x4 m4;
	m4s =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	m4.m[0][0] = 1 / m4s * (
		m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]
		);
	m4.m[0][1] = 1 / m4s * (
		-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	m4.m[0][2] = 1 / m4s * (
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	m4.m[0][3] = 1 / m4s * (
		-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	//2
	m4.m[1][0] = 1 / m4s * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][1] = 1 / m4s * (+m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][2] = 1 / m4s * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	m4.m[1][3] = 1 / m4s * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);



	m4.m[2][0] = 1 / m4s * (
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]
		);
	m4.m[2][1] = 1 / m4s * (
		-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	m4.m[2][2] = 1 / m4s * (
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	m4.m[2][3] = 1 / m4s * (
		-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);


	m4.m[3][0] = 1 / m4s * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][1] = 1 / m4s * (+m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][2] = 1 / m4s * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	m4.m[3][3] = 1 / m4s * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return m4;
}


// 出力ウインドウにだす
void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	// これからシェーダーコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompilerShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
		filePath.c_str(),// コンパイル対象のhlslファイル名
		L"-E", L"main",// エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,// ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",// デバッグ用の情報を埋め込む
		L"-Od",// 最適化を外しておく
		L"-Zpr",// メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,// 読み込んだファイル
		arguments,// コンパイルオプション
		_countof(arguments),// コンパイルオプションの数
		includeHandler,// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)// コンパイル結果
	);

	assert(SUCCEEDED(hr));

	// 警告, エラーが出たらログに出してみる
	IDxcBlobUtf8* shaderError = nullptr;

	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告, エラーダメ絶対
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}




D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;

}


D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;

}