#include "Model.h"


void Model::Initialize(DirectXCommon* dir_, Triangle* tri_) {
	// モデル読み込み
	modelData = LoadObjFile("resources", "axis.obj");
	DirectX::ScratchImage mipImages2 = tri_->LoadTexture(modelData.material.textureFilePath);

	Model::CreateVertexResource(dir_,tri_);
	Model::CreateMaterialResource(dir_,tri_);
	Model::CreateWVPResource(dir_,tri_);

	transform = { { 0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,-0.5f,1.0f} };
	uvTransform = { {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f},{0.0f, 0.0f, 0.0f}, };

	// Lightingするか
	materialData->enableLighting = true;
}


void Model::Update(const Matrix4x4& transformationMatrixData) {
	wvpData->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	wvpData->World = Multiply(wvpData->World, transformationMatrixData);
	wvpData->WVP = wvpData->World;

	Matrix4x4 uvtransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMatrix = Multiply(uvtransformMatrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvtransformMatrix = Multiply(uvtransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData->uvTransform = uvtransformMatrix;
}


void Model::Draw(DirectXCommon* dir_, Triangle* tri_) {
	// コマンドを積む
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferView); // VBVを設定
	// マテリアルCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// TransformationMatrixCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, tri_->GettextureSrvHandleGPU());
	if (isModel == true) {
		// 描画(DrawCall/ドローコール)
		dir_->GetCommandList_()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
	}

	ImGui::Begin("Model2");

	ImGui::Checkbox("IsModel", &isModel);

	ImGui::SliderAngle("Rotate.y ", &transform.rotate.y);
	ImGui::DragFloat3("Transform", &transform.translate.x, 0.01f, -10.0f, 10.0f);

	ImGui::DragFloat2("UVTransform", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransform.rotate.z);
	ImGui::End();
}

void Model::Release() {
}

void Model::CreateVertexResource(DirectXCommon* dir_, Triangle* tri_) {
	// 頂点用のリソースを作る。
	vertexResource = tri_->CreateBufferResource(dir_->GetDevice().Get(), sizeof(VertexData) * modelData.vertices.size());

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	vertexData = nullptr;

	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	// 頂点データをリソースにコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Model::CreateMaterialResource(DirectXCommon* dir_, Triangle* tri_) {
	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = tri_->CreateBufferResource(dir_->GetDevice().Get(), sizeof(Material));
	// マテリアルにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 白を設定
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };

	materialData->uvTransform = MakeIdentity4x4();
}

void Model::CreateWVPResource(DirectXCommon* dir_, Triangle* tri_) {
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = tri_->CreateBufferResource(dir_->GetDevice().Get(), sizeof(TransformationMatrix));

	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	// 単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
}

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 必要な変数の宣言
	ModelData modelData; // 構築するModelData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの

	// ファイルを開くために開く
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.z *= -1.0f;
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
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string  vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/uv/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t  elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position, texcoord, normal };
			}
			// 頂点を逆順で登録することで、回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// materialtemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	return modelData;
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	// 必要な変数の宣言
	MaterialData materialData; // 構築するModelData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

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