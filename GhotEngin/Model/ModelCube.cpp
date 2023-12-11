#include "ModelCube.h"

void ModelCube::Initialize(Model* state) {

	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 8);

	VBV = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 8, 8);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * 36);
	IBV = CreateResource::CreateIndexBufferView(resource_.indexResource, sizeof(uint32_t) * 36);

	// インデックスリソースにデータを書き込む
	uint32_t* indexData = nullptr;
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

#pragma region 頂点データとインデックスデータ

	// 1
	vertexData[0].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal.x = vertexData[0].position.x;
	vertexData[0].normal.y = vertexData[0].position.y;
	vertexData[0].normal.z = vertexData[0].position.z;
	// 2
	vertexData[1].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData[1].texcoord = { 1.0f,1.0f };
	vertexData[1].normal.x = vertexData[1].position.x;
	vertexData[1].normal.y = vertexData[1].position.y;
	vertexData[1].normal.z = vertexData[1].position.z;
	// 3
	vertexData[2].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,0.0f };
	vertexData[2].normal.x = vertexData[2].position.x;
	vertexData[2].normal.y = vertexData[2].position.y;
	vertexData[2].normal.z = vertexData[2].position.z;
	// 4
	vertexData[3].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[3].texcoord = { 0.0f,0.0f };
	vertexData[3].normal.x = vertexData[3].position.x;
	vertexData[3].normal.y = vertexData[3].position.y;
	vertexData[3].normal.z = vertexData[3].position.z;
	// 5
	vertexData[4].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[4].texcoord = { 0.0f,0.0f };
	vertexData[4].normal.x = vertexData[4].position.x;
	vertexData[4].normal.y = vertexData[4].position.y;
	vertexData[4].normal.z = vertexData[4].position.z;
	// 6
	vertexData[5].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[5].texcoord = { 1.0f,0.0f };
	vertexData[5].normal.x = vertexData[5].position.x;
	vertexData[5].normal.y = vertexData[5].position.y;
	vertexData[5].normal.z = vertexData[5].position.z;
	// 7
	vertexData[6].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[6].texcoord = { 1.0f,1.0f };
	vertexData[6].normal.x = vertexData[6].position.x;
	vertexData[6].normal.y = vertexData[6].position.y;
	vertexData[6].normal.z = vertexData[6].position.z;
	// 8
	vertexData[7].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[7].texcoord = { 0.0f,1.0f };
	vertexData[7].normal.x = vertexData[7].position.x;
	vertexData[7].normal.y = vertexData[7].position.y;
	vertexData[7].normal.z = vertexData[7].position.z;

	// インデックスデータ
	// 正面
	indexData[0] = 1;    indexData[1] = 0;   indexData[2] = 3;
	indexData[3] = 3;	 indexData[4] = 2;	 indexData[5] = 1;
	// 背面
	indexData[6] = 4;	 indexData[7] = 5;	 indexData[8] = 6;
	indexData[9] = 6;	 indexData[10] = 7;	 indexData[11] = 4;
	// 右側面
	indexData[12] = 5;	 indexData[13] = 1;	 indexData[14] = 2;
	indexData[15] = 2;	 indexData[16] = 6;	 indexData[17] = 5;
	// 左側面
	indexData[18] = 0;	 indexData[19] = 4;  indexData[20] = 7;
	indexData[21] = 7;	 indexData[22] = 3;	 indexData[23] = 0;
	// 底面
	indexData[24] = 5;	 indexData[25] = 4;	 indexData[26] = 0;
	indexData[27] = 0;	 indexData[28] = 1;	 indexData[29] = 5;
	// 上面
	indexData[30] = 7;	 indexData[31] = 6;	 indexData[32] = 2;
	indexData[33] = 2;	 indexData[34] = 3;	 indexData[35] = 7;

#pragma endregion

	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	Material* materialData = nullptr;
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = false;

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

void ModelCube::Draw(WorldTransform worldTransform, ViewProjection viewProjection, uint32_t texHandle) {

	worldTransform.TransferMatrix(resource_.wvpResource, viewProjection);

	Property property = GraphicsPipeline::GetInstance()->GetPSO().Object3D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV); // VBVを設定
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV); // IBVの設定
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
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0);

}