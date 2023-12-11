#include "ModelSphere.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="state"></param>
void ModelSphere::Initialize(Model* state)
{

	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * (kSubdivision * kSubdivision * 6));

	VBV = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * (kSubdivision * kSubdivision * 6), kSubdivision * kSubdivision * 6);


	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

#pragma region 頂点データ

	// 経度分割1つ分の角度
	const float pi = (float)M_PI;
	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	// 緯度分割1つ分の角度
	const float kLatEvery = pi / float(kSubdivision);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex; // θ
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery; // φ
			// texcoord
			float u = float(lonIndex) / float(kSubdivision);
			float v = 1.0f - float(latIndex) / float(kSubdivision);
			float uv = 1.0f / float(kSubdivision);
			// θd, φd
			float latD = pi / kSubdivision;
			float lonD = (2.0f * pi) / kSubdivision;
			// 基準点a
			vertexData[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData[start].position.y = std::sin(lat);
			vertexData[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord = { u,v };
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;
			// 基準点b
			vertexData[start + 1].position.x = std::cos(lat + latD) * std::cos(lon);
			vertexData[start + 1].position.y = std::sin(lat + latD);
			vertexData[start + 1].position.z = std::cos(lat + latD) * std::sin(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start + 1].texcoord = { u ,v - uv };
			vertexData[start + 1].normal.x = vertexData[start + 1].position.x;
			vertexData[start + 1].normal.y = vertexData[start + 1].position.y;
			vertexData[start + 1].normal.z = vertexData[start + 1].position.z;
			// 基準点c
			vertexData[start + 2].position.x = std::cos(lat) * std::cos(lon + lonD);
			vertexData[start + 2].position.y = std::sin(lat);
			vertexData[start + 2].position.z = std::cos(lat) * std::sin(lon + lonD);
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texcoord = { u + uv ,v };
			vertexData[start + 2].normal.x = vertexData[start + 2].position.x;
			vertexData[start + 2].normal.y = vertexData[start + 2].position.y;
			vertexData[start + 2].normal.z = vertexData[start + 2].position.z;
			// 基準点d
			vertexData[start + 3].position.x = std::cos(lat + latD) * std::cos(lon + lonD);
			vertexData[start + 3].position.y = std::sin(lat + latD);
			vertexData[start + 3].position.z = std::cos(lat + latD) * std::sin(lon + lonD);
			vertexData[start + 3].position.w = 1.0f;
			vertexData[start + 3].texcoord = { u + uv ,v - uv };
			vertexData[start + 3].normal.x = vertexData[start + 3].position.x;
			vertexData[start + 3].normal.y = vertexData[start + 3].position.y;
			vertexData[start + 3].normal.z = vertexData[start + 3].position.z;
			// 基準点c2
			vertexData[start + 4].position.x = std::cos(lat) * std::cos(lon + lonD);
			vertexData[start + 4].position.y = std::sin(lat);
			vertexData[start + 4].position.z = std::cos(lat) * std::sin(lon + lonD);
			vertexData[start + 4].position.w = 1.0f;
			vertexData[start + 4].texcoord = { u + uv ,v };
			vertexData[start + 4].normal.x = vertexData[start + 4].position.x;
			vertexData[start + 4].normal.y = vertexData[start + 4].position.y;
			vertexData[start + 4].normal.z = vertexData[start + 4].position.z;
			// 基準点b2
			vertexData[start + 5].position.x = std::cos(lat + latD) * std::cos(lon);
			vertexData[start + 5].position.y = std::sin(lat + latD);
			vertexData[start + 5].position.z = std::cos(lat + latD) * std::sin(lon);
			vertexData[start + 5].position.w = 1.0f;
			vertexData[start + 5].texcoord = { u ,v - uv };
			vertexData[start + 5].normal.x = vertexData[start + 5].position.x;
			vertexData[start + 5].normal.y = vertexData[start + 5].position.y;
			vertexData[start + 5].normal.z = vertexData[start + 5].position.z;

		}
	}

	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	Material* materialData = nullptr;
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true;

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// データを書き込む
	DirectionalLight* directionalLightData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData->intensity = 1.0f;

	state;
}

/// <summary>
/// 描画
/// </summary>
/// <param name="state"></param>
/// <param name="viewProjection"></param>
/// <param name="texHandle"></param>
void ModelSphere::Draw(WorldTransform worldTransform, ViewProjection viewProjection, uint32_t texHandle)
{

	worldTransform.TransferMatrix(resource_.wvpResource, viewProjection);

	Property property = GraphicsPipeline::GetInstance()->GetPSO().Object3D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(texHandle));
	// 平行光源
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_.directionalLightResource->GetGPUVirtualAddress());
	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawInstanced(kSubdivision * kSubdivision * 6, 1, 0, 0);
}