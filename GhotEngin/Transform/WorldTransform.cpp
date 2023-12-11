#include "WorldTransform.h"
#include "CameraRole.h"

void WorldTransform::Initialize() {

	matWorld = MakeIdentityMatrix();

}

void WorldTransform::TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& viewProjection) {

	TransformationMatrix* wvp = {};
	matWorld = Multiply(matWorld, Multiply(viewProjection.matView, viewProjection.matProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = matWorld;
	wvp->World = worldMatrix;

}

void WorldTransform::STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& viewProjection)
{
	TransformationMatrix* wvp = {};
	sMatWorld = Multiply(matWorld, Multiply(viewProjection.sMatView, viewProjection.sMatProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = sMatWorld;
	wvp->World = worldMatrix;
}

void WorldTransform::UpdateMatrix() {

	matWorld = MakeAffineMatrix(scale, rotate, translate);
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	if (parent) {
		matWorld = Multiply(matWorld, parent->matWorld);
	}

}