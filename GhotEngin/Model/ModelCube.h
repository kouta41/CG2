#pragma once
#include "IModelState.h"

class ModelCube : public IModelState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	void Initialize(Model* state)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	/// <param name="texHandle"></param>
	void Draw(WorldTransform worldTransform, ViewProjection viewProjection, uint32_t texHandle)override;

private:

	D3D12_INDEX_BUFFER_VIEW IBV{};
	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
};