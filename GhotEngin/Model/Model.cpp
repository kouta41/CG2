#include "Model.h"

Model::~Model()
{
	delete state_;
}

/// <summary>
/// 初期化
/// </summary>
void Model::Initialize(IModelState* state)
{
	state_ = state;
	state_->Initialize(this);

}

void Model::InitializeObj(const std::string& filename)
{

	modelData_ = LoadObjFile("resources", filename);

	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// VertexBufferView
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	objVertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	objVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	objVertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	materialData_->shininess = 70.0f;

	resource_.cameraResource = CreateResource::CreateBufferResource(sizeof(Camera));
	resource_.cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f,10.0f,-40.0f };

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));

	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;

	resource_.pointLightResource = CreateResource::CreateBufferResource(sizeof(PointLight));
	resource_.pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,10.0f,0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 12.0f;
	pointLightData_->decay = 0.6f;

}

/// <summary>
/// モデル生成
/// </summary>
/// <returns></returns>
Model* Model::Create(IModelState* state)
{
	Model* model_ = new Model;
	model_->Initialize(state);

	return model_;
}

// <summary>
// Obj
// </summary>
// <param name="filename"></param>
// <returns></returns>
Model* Model::CreateObj(const std::string& filename) {

	Model* model_ = new Model;
	model_->InitializeObj(filename);

	return model_;
}


void Model::Draw(WorldTransform worldTransform, CameraRole cameraRole, uint32_t texHandle)
{
	state_->Draw(worldTransform, cameraRole, texHandle);
}

void Model::Draw(WorldTransform worldTransform, CameraRole cameraRole)
{

	worldTransform.TransferMatrix(resource_.wvpResource, cameraRole);

	//Property property = GraphicsPipeline::GetInstance()->GetPSO().Object3D;
	Property property = GraphicsPipeline::GetInstance()->GetPSO().PointLight;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectX12::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectX12::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectX12::GetCommandList()->IASetVertexBuffers(0, 1, &objVertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectX12::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectX12::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	DirectX12::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	DirectX12::GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	DirectX12::GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_.directionalLightResource->GetGPUVirtualAddress());
	// カメラ用
	DirectX12::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.cameraResource->GetGPUVirtualAddress());
	// ポイントライト用
	DirectX12::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.pointLightResource->GetGPUVirtualAddress());
	// 描画。(DrawCall/ドローコール)。
	DirectX12::GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}


ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 必要な変数を宣言
	ModelData modelData; // 構築するModelData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの
	// ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // 開けなかったら止める
	// ファイルを読む
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのindexは「位置/UV/法線」で格納されているので、分解してindexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのindexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				triangle[faceVertex] = { position, texcoord, normal };
			}
			// 逆順で頂点を登録する
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	return modelData;
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	// 変数
	MaterialData materialData; // 構築するmaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める
	// ファイルを読み、materialdataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}
