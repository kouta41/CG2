#include "Triangle.h"

void Triangle::Initialize(DirectXCommon* dir_, Vector4* pos,WinApp* window) {
	window_ = window;

	Triangle::CreateMaterialResource(dir_);
	Triangle::CreateWVPResource(dir_);

	Triangle::CreateVertexResource(dir_, pos);
	Triangle::Create2DSpriteResource(dir_);
	Triangle::CreateSphereResoure(dir_);

#pragma region Light
	directionalLightResource = CreateBufferResource(dir_->GetDevice().Get(), sizeof(DirectionalLight));
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
#pragma endregion ライト

#pragma region OBJ
	// モデル読み込み
	modelData = LoadObjFile("resources", "axis.obj");
	DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);

	vertexResourceObj = CreateBufferResource(dir_->GetDevice().Get(), sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferViewObj.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj.StrideInBytes = sizeof(VertexData);
	vertexResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj));
	std::memcpy(vertexDataObj, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	materialResourceObj = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Material));
	materialResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj));
	// 白を設定
	materialDataObj->color = { 1.0f,1.0f,1.0f,1.0f };
	materialDataObj->uvTransform = MakeIdentity4x4();

	wvpResourceObj= CreateBufferResource(dir_->GetDevice().Get(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResourceObj->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataObj));
	// 単位行列を書き込んでおく
	wvpDataObj->WVP = MakeIdentity4x4();
	//ライティングをしない
	materialDataObj->enableLighting = true;

	// Lightingするか
	//materialDataObj->enableLighting = true;
#pragma endregion オブジェクト

	Triangle::LoadTexture(dir_);
}

void Triangle::Update() {
}

void Triangle::Draw(DirectXCommon* dir_) {
//	transform_.rotate.y += 0.01f;
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTranformTriAngle.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTranformTriAngle.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTranformTriAngle.translate));
	materialData->uvTransform = uvTransformMatrix;

	Matrix4x4 WorldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//単位行列を書き込んでおく
	*wvpData = WorldMatrix;


	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	dir_->GetCommandList_()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	dir_->GetCommandList_()->DrawInstanced(6, 1, 0, 0);
	
	
}

void Triangle::DrawSprite(DirectXCommon* dir_) {

	materialDataSprite->uvTransform = MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvtransformSprite_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvtransformSprite_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvtransformSprite_.translate));
	materialDataSprite->uvTransform = uvTransformMatrix;
	//WVPを書き込むためのアドレス取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 ProjectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, (float)window_->GetkClientWidth(), (float)window_->GetkClientHeight(), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, ProjectionMatrixSprite));
	//
	transformationMatrixDataSprite->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite->World = MakeIdentity4x4();
	
	//Spriteの描画。変更が必要なものだけ変更
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	dir_->GetCommandList_()->IASetIndexBuffer(&indexBufferViewSprite);
	//TransfoormationMatrixCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	dir_->GetCommandList_()->DrawInstanced(6, 1, 0, 0);
	// 描画(DrawCall/ドローコール)。6個のインデックスを使用し1つのいインスタンスを描画
	dir_->GetCommandList_()->DrawIndexedInstanced(6, 1, 0, 0, 0);


	ImGui::Begin("SpriteDraw");
	ImGui::SliderFloat3("SpriteDraw", &transformSprite_.translate.x, -1000.0f, 1000.0f);

	ImGui::DragFloat2("UVTranslate", &uvtransformSprite_.translate.x, 0.01f, -10.0f,10.0f);
	ImGui::DragFloat2("UVScale", &uvtransformSprite_.scale.x, 0.01f,-10.0f,10.0f);
	ImGui::SliderAngle("UvRotate", &uvtransformSprite_.rotate.z);

	ImGui::End();

}

void Triangle::DrawSphere(DirectXCommon* dir_) {
	//transformSphere.rotate.y-=0.1f;
	//書き込むためのアドレスを取得
	transformationMatrixResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere));
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
	transformationMatrixDataSphere->WVP = worldMatrixSphere;
	transformationMatrixDataSphere->World = MakeIdentity4x4();


	dir_->GetCommandList_()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//色用のCBufferの場所を特定
//	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResourceSphere->GetGPUVirtualAddress());

	//頂点
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere);
	//WVP
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere->GetGPUVirtualAddress());
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU : textureSrvHandleGPU2);
	//Light
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	dir_->GetCommandList_()->DrawInstanced(kSubdivision * kSubdivision * 6, 1, 0, 0);

	ImGui::Begin("Sphere");
	ImGui::SliderFloat3("ScaleSphere", &transformSphere.scale.x, 0.1f, 30, "%.3f");
	ImGui::SliderFloat3("RotateSphere", &transformSphere.rotate.x, -7, 7, "%.3f");
	ImGui::SliderFloat3("TranslateSphere",&transformSphere.translate.x, -1, 1, "%.3f");
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	ImGui::SliderFloat3("color", &directionalLightData->color.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("direction", &directionalLightData->direction.x, -10.0f, 10.0f);
	ImGui::SliderFloat("intensity", &directionalLightData->intensity, -10.0f, 10.0f);
	ImGui::End();

	ImGui::Begin("Light");
	float ImGuidirectionalLightColor[Vector3D] = { directionalLightData->color.x,directionalLightData->color.y ,directionalLightData->color.z };
	ImGui::SliderFloat3("LightColor", ImGuidirectionalLightColor, 0, 1, "%.3f");
	directionalLightData->color.x = ImGuidirectionalLightColor[x];
	directionalLightData->color.y = ImGuidirectionalLightColor[y];
	directionalLightData->color.z = ImGuidirectionalLightColor[z];
	float ImGuidirectionalLightdirection[Vector3D] = { directionalLightData->direction.x,directionalLightData->direction.y,directionalLightData->direction.z };
	ImGui::SliderFloat3("Lightpotision", ImGuidirectionalLightdirection, -1, 1, "%.3f");
	directionalLightData->direction.x = ImGuidirectionalLightdirection[x];
	directionalLightData->direction.y = ImGuidirectionalLightdirection[y];
	directionalLightData->direction.z = ImGuidirectionalLightdirection[z];
	ImGui::End();
}

void Triangle::DrawOBJ(DirectXCommon* dir_, const Matrix4x4& transformationMatrixData) {
	wvpDataObj->World = MakeAffineMatrix(transformObj.scale, transformObj.rotate, transformObj.translate);
	//wvpDataObj->World = Multiply(wvpDataObj->World, transformationMatrixData);
	wvpDataObj->WVP = wvpDataObj->World;

	Matrix4x4 uvtransformMatrix = MakeScaleMatrix(uvTransformObj.scale);
	uvtransformMatrix = Multiply(uvtransformMatrix, MakeRotateZMatrix(uvTransformObj.rotate.z));
	uvtransformMatrix = Multiply(uvtransformMatrix, MakeTranslateMatrix(uvTransformObj.translate));
	materialData->uvTransform = uvtransformMatrix;

	

	//頂点
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferViewObj);
	//色用のCBufferの場所を特定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResourceObj->GetGPUVirtualAddress());
	//WVP
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1,materialResourceObj->GetGPUVirtualAddress());
	//テクスチャ
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	//Light
	//dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	dir_->GetCommandList_()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

	ImGui::Begin("Model");
	ImGui::SliderAngle(".Rotate.y ", &transformObj.rotate.y);
	ImGui::SliderFloat3("translate", &transformObj.translate.x, -1.0f, 1.0f);
	ImGui::DragFloat2("UVTransform", &uvTransformObj.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformObj.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformObj.rotate.z);
	ImGui::End();
}

void Triangle::Release() {
	
}

void Triangle::CreateVertexResource(DirectXCommon* dir_, Vector4* pos) {
	// 頂点用のリソースを作る。今回はcolor1つ分のサイズを用意する
	vertexResource = CreateBufferResource(dir_->GetDevice().Get(), sizeof(VertexData) * 6);
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 左下
	vertexData[0].position = pos[0];
	vertexData[0].texcoord = { 0.0f, 1.0f };
	// 上
	vertexData[1].position = pos[1];
	vertexData[1].texcoord = { 0.5f, 0.0f };

	// 右上
	vertexData[2].position = pos[2];
	vertexData[2].texcoord = { 1.0f, 1.0f };

	// 左下2
	vertexData[3].position = pos[0];
	vertexData[3].texcoord = { 0.0f, 1.0f };
	// 上2
	vertexData[4].position = pos[1];
	vertexData[4].texcoord = { 0.5f, 0.0f };

	// 右上2
	vertexData[5].position = pos[2];
	vertexData[5].texcoord = { 1.0f, 1.0f };


	//WVP用のリソースを作る。matrix4x4 一つ分サイズ分を用意する
	wvpResource = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Matrix4x4));
	//データを書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 今回は赤を書き込んでみる
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdentity4x4();


	//DepthStencilTextureをウィンドウのサイズで作成
	depthStencilResource = CreateDepthStencilTextureResource(dir_->GetDevice().Get(), window_->GetkClientWidth(), window_->GetkClientHeight());

}

void Triangle::Create2DSpriteResource(DirectXCommon* dir_) {
	//Sprite用の頂点リソースを作る
	vertexResourceSprite = CreateBufferResource(dir_->GetDevice().Get(), sizeof(VertexData) * 6);
	materialResourceSprite = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Material) * 6);

	transformationMatrixResourceSprite = CreateBufferResource(dir_->GetDevice().Get(), sizeof(TransformationMatrix));
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) *6 ;
	//1頂点当たりのサイズ
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);

	indexResourceSprite = CreateBufferResource(dir_->GetDevice().Get(), sizeof(uint32_t) * 12);

	//リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;


	//頂点データを設定
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	//1枚目の三角形
	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	//2枚目の三角形
	vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexDataSprite[4].texcoord = { 1.0f,0.0f };
	vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };

	//インデックスリソースにデータを書き込む
	indexResourceSprite->Map(0,nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;		indexDataSprite[1] = 1;		indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;		indexDataSprite[4] = 3;		indexDataSprite[5] = 2;


	//色の書き込み
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	materialDataSprite->color = {1.0f,1.0f,1.0f,1.0f,};
	//ライティングをしない
	materialDataSprite->enableLighting = false;

	

}

void Triangle::CreateSphereResoure(DirectXCommon* dir_) {
	vertexResourceSphere = CreateBufferResource(dir_->GetDevice().Get(), sizeof(VertexData) * 6 * kSubdivision * kSubdivision);
	//リソースの先頭のアドレス
	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	//使用する頂点サイズ
	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision * kSubdivision;
	//1頂点あたりのアドレス
	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);


	//頂点データを設定
	vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere));
	//経度分割の1つ分の角度　φ 横
	const float kLonEvery = float(std::numbers::pi) * 2.0f / float(kSubdivision);
	//緯度分割の1つ分の角度　θ 縦
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);
	for (int latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;
		float uvLength = 1.0f / kSubdivision;
		for (int lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			float u = float(lonIndex) / float(kSubdivision);
			float v = 1.0f - float(latIndex) / float(kSubdivision);
#pragma region TriAngle 1
			//点A(左下)
			vertexDataSphere[start].position.x = cos(lat) * cos(lon) + sphere.center.x;
			vertexDataSphere[start].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere[start].position.z = cos(lat) * sin(lon) + sphere.center.z;
			vertexDataSphere[start].position.w = 1.0f;
			vertexDataSphere[start].texcoord = { u ,v + uvLength };
			vertexDataSphere[start].normal.x = vertexDataSphere[start].position.x;
			vertexDataSphere[start].normal.y = vertexDataSphere[start].position.y;
			vertexDataSphere[start].normal.z = vertexDataSphere[start].position.z;
			//点B(左上)
			vertexDataSphere[start + 1].position.x = cos(lat + kLatEvery) * cos(lon) + sphere.center.x;
			vertexDataSphere[start + 1].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere[start + 1].position.z = cos(lat + kLatEvery) * sin(lon) + sphere.center.z;
			vertexDataSphere[start + 1].position.w = 1.0f;
			vertexDataSphere[start + 1].texcoord = { u,v };
			vertexDataSphere[start + 1].normal.x = vertexDataSphere[start + 1].position.x;
			vertexDataSphere[start + 1].normal.y = vertexDataSphere[start + 1].position.y;
			vertexDataSphere[start + 1].normal.z = vertexDataSphere[start + 1].position.z;
			//点C(右下)
			vertexDataSphere[start + 2].position.x = cos(lat) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere[start + 2].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere[start + 2].position.z = cos(lat) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere[start + 2].position.w = 1.0f;
			vertexDataSphere[start + 2].texcoord = { u + uvLength,v + uvLength };
			vertexDataSphere[start + 2].normal.x = vertexDataSphere[start + 2].position.x;
			vertexDataSphere[start + 2].normal.y = vertexDataSphere[start + 2].position.y;
			vertexDataSphere[start + 2].normal.z = vertexDataSphere[start + 2].position.z;
#pragma endregion 1枚目
#pragma region TriAngle 2
			//点D(右上)
			vertexDataSphere[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere[start + 3].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere[start + 3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere[start + 3].position.w = 1.0f;
			vertexDataSphere[start + 3].texcoord = { u + uvLength,v };
			vertexDataSphere[start + 3].normal.x = vertexDataSphere[start + 3].position.x;
			vertexDataSphere[start + 3].normal.y = vertexDataSphere[start + 3].position.y;
			vertexDataSphere[start + 3].normal.z = vertexDataSphere[start + 3].position.z;
			//点C(右下)
			vertexDataSphere[start + 4].position.x = cos(lat) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere[start + 4].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere[start + 4].position.z = cos(lat) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere[start + 4].position.w = 1.0f;
			vertexDataSphere[start + 4].texcoord = { u + uvLength,v + uvLength };
			vertexDataSphere[start + 4].normal.x = vertexDataSphere[start + 4].position.x;
			vertexDataSphere[start + 4].normal.y = vertexDataSphere[start + 4].position.y;
			vertexDataSphere[start + 4].normal.z = vertexDataSphere[start + 4].position.z;
			//点B(左上)
			vertexDataSphere[start + 5].position.x = cos(lat + kLatEvery) * cos(lon) + sphere.center.x;
			vertexDataSphere[start + 5].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere[start + 5].position.z = cos(lat + kLatEvery) * sin(lon) + sphere.center.z;
			vertexDataSphere[start + 5].position.w = 1.0f;
			vertexDataSphere[start + 5].texcoord = { u,v };
			vertexDataSphere[start + 5].normal.x = vertexDataSphere[start + 5].position.x;
			vertexDataSphere[start + 5].normal.y = vertexDataSphere[start + 5].position.y;
			vertexDataSphere[start + 5].normal.z = vertexDataSphere[start + 5].position.z;


#pragma endregion 2枚目
		}
	}
	vertexDataSphere[0].normal = { 0.0f,0.0f,-1.0f };

	materialResourceSphere = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Material));
	materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));

	//ライティングをする
	materialDataSphere->enableLighting = true;
	materialDataSphere->color = { 1.0f,1.0f,1.0f,1.0f};

	transformationMatrixResourceSphere = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Matrix4x4));
	
	

}




void Triangle::LoadTexture(DirectXCommon* dir_) {

	// Textureを読んで転送する(1枚目)
	DirectX::ScratchImage mipImages = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(dir_->GetDevice().Get(), metadata);
	UploadTextureData(textureResource.Get(), mipImages);

	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU = dir_->GetsrvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dir_->GetsrvDescriptorHeap_()->GetGPUDescriptorHandleForHeapStart();

	// 先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// SRVの生成
	dir_->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);

	//DSVの設定
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DSVHeapの先頭にDSVを作る
	dir_->GetDevice()->CreateDepthStencilView(depthStencilResource, &dsvDesc, dir_->GetdsvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart());

	//DescriptorSizeを取得しておく
	const uint32_t desriptorSizeSRV = dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t desriptorSizeRTV = dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t desriptorSizeDSV = dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	GetCPUDescriptorHandle(dir_->GetrtvDescriptorHeap_(), desriptorSizeRTV, 0);



	//二枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages2 = LoadTexture("resources/monsterBall.png");
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	ID3D12Resource* textureResoure2 = CreateTextureResource(dir_->GetDevice().Get(), metadata2);
	UploadTextureData(textureResoure2, mipImages2);

	//meataDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	//SRVを作成するDescriptorHeapの場所に決める
	textureSrvHandleCPU2 = GetCPUDescriptorHandle(dir_->GetsrvDescriptorHeap_(), desriptorSizeSRV, 2);
	textureSrvHandleGPU2 = GetGPUDescriptorHandle(dir_->GetsrvDescriptorHeap_(), desriptorSizeSRV, 2);
	//SRVを生成
	dir_->GetDevice()->CreateShaderResourceView(textureResoure2, &srvDesc2, textureSrvHandleCPU2);

}

MaterialData Triangle::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + '/' + filename);//ファイルを開く
	assert(file.is_open());//開けなかったら止める
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

ModelData Triangle::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;	//構築するModelData
	std::vector<Vector4> positions;	//位置
	std::vector<Vector3> normals;	//法線
	std::vector<Vector2> texcoords;	//テクスチャ座標
	std::string line;	//ファイルから読んだ1桁を格納するもの
	std::ifstream file(directoryPath + "/" + filename);	//ファイルを開く
	assert(file.is_open());	//開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;	//先頭の識別子を読む

		//identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			//面は三角形限定
			//VertexData triamgle[3];
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
				//	triamgle[faceVertex] = { position,texcoord,normal };

			}

		}

	}
	return modelData;
}
void Triangle::CreateMaterialResource(DirectXCommon* dir_) {
	//// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Vector4));

	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	materialBufferView.BufferLocation = materialResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	materialBufferView.SizeInBytes = sizeof(Vector4);
	// 1頂点あたりのサイズ
	materialBufferView.StrideInBytes = sizeof(Vector4);

	// マテリアルにデータを書き込む

	
}

void Triangle::CreateWVPResource(DirectXCommon* dir_) {
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = CreateBufferResource(dir_->GetDevice().Get(), sizeof(Matrix4x4));
	// データを書き込む
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	// 単位行列を書き込んでおく
	*wvpData = MakeIdentity4x4();
}

void Triangle::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	// meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// Mipmapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		// MipMapLevelを指示して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);

		assert(SUCCEEDED(hr));
	}
}

ID3D12Resource* Triangle::CreateBufferResource(ID3D12Device* device, size_t sizeInbytes) {
	ID3D12Resource* Resource = nullptr;

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	// 頂点リソース用のヒープの設定
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeapを使う

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInbytes;
	// バッファの場合はこれらは1にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	hr_ = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Resource));

	assert(SUCCEEDED(hr_));

	return Resource;
}

// textureResource
ID3D12Resource* Triangle::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width); // textureの幅
	resourceDesc.Height = UINT(metadata.height); // textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列textureの配列数
	resourceDesc.Format = metadata.format; // textureのformat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // textureの次元数。普段使っているのは2次元

	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース番がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// Resourceの生成
	ID3D12Resource* resource = nullptr;

	hr_ = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ, // 初回のResourceState。Textureは基本読むだけ
		nullptr, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ

	assert(SUCCEEDED(hr_));

	return resource;
}

ID3D12Resource* Triangle::CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width; // textureの幅
	resourceDesc.Height = height; // textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//DepthStencilとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//VRAM上に作る

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//フォーマット。Resourceと合わせる

	// Resourceの生成
	ID3D12Resource* resource = nullptr;

	hr_ = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ

	assert(SUCCEEDED(hr_));

	return resource;

}
DirectX::ScratchImage Triangle::LoadTexture(const std::string& filePath) {

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);;
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}


