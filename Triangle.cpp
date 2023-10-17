#include "Triangle.h"

void Triangle::Initialize(DirectXCommon* dir,WinApp* window) {
	window_ = window;
	//pos_ = pos;
	dir_ = dir;
#pragma region TriAngle
	vertexResource = CreateBufferResource(sizeof(VertexData) * 6);
	materialResource = CreateBufferResource(sizeof(Vector4) * 3);
	wvpResource = CreateBufferResource(sizeof(Matrix4x4));
	MakeVertexBufferView();
#pragma endregion 三角形
#pragma region Sprite
	vertexResourceSprite = CreateBufferResource(sizeof(VertexData) * 6);
	materialResourceSprite = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceSprite = CreateBufferResource(sizeof(TransformationMatrix));
	MakeVertexBufferViewSprite();
#pragma endregion スプライト
#pragma region Sphere
	vertexResourceSphere = CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision * kSubdivision);
	materialResourceSphere = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceSphere = CreateBufferResource(sizeof(TransformationMatrix));
	MakeVertexBufferViewSphere();
#pragma endregion 球
#pragma region Light
	directionalLightResource = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
#pragma endregion ライト

	Triangle::LoadTexture();
	//Triangle::Create2DSpriteResource();
	//Triangle::CreateSphereResoure();
	
}

void Triangle::Update() {
}

void Triangle::Draw(Vector4* pos,const Vector4& color, const Matrix4x4& ViewMatrix) {

	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 左下
	vertexData[0].position = pos_[0];
	vertexData[0].texcoord = { 0.0f, 1.0f };
	// 上
	vertexData[1].position = pos_[1];
	vertexData[1].texcoord = { 0.5f, 0.0f };

	// 右上
	vertexData[2].position = pos_[2];
	vertexData[2].texcoord = { 1.0f, 1.0f };

	// 左下2
	vertexData[3].position = pos_[0];
	vertexData[3].texcoord = { 0.0f, 1.0f };
	// 上2
	vertexData[4].position = pos_[1];
	vertexData[4].texcoord = { 0.5f, 0.0f };

	// 右上2
	vertexData[5].position = pos_[2];
	vertexData[5].texcoord = { 1.0f, 1.0f };


	//データを書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色情報を書き込む
	*materialData = color;

	Matrix4x4 WorldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	//WVPを書き込むアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

//	transform_.rotate.y += 0.01f;
	//単位行列を書き込んでおく
	*wvpData = Multiply(WorldMatrix, ViewMatrix);


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
void Triangle::MakeVertexBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Triangle::DrawSprite() {

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

	//色の書き込み
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	//materialDataSprite->color = color;
	//ライティングをしない
	materialDataSprite->enableLighting = false;


	//書き込むためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//Sprite用のworldViewProjectionMatrixを作る
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(window_->GetkClientWidth()), float(window_->GetkClientHeight()), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixDataSprite->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite->World = MakeIdentity4x4();

	dir_->GetCommandList_()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Spriteの描画。変更が必要なものだけ変更
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	//TransfoormationMatrixCBufferの場所を設定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	//Light
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	dir_->GetCommandList_()->DrawInstanced(6, 1, 0, 0);

	ImGui::Begin("SpriteDraw");
	ImGui::SliderFloat3("SpriteDraw", &transformSprite_.translate.x, -1000.0f, 1000.0f);
	ImGui::End();

}
void Triangle::MakeVertexBufferViewSprite()
{
	//リソースの先頭のアドレス
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	//使用する頂点サイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのアドレス
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);
}


void Triangle::DrawSphere(const Matrix4x4& ViewMatrix, const Vector4& color) {
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
	
	materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));

	//ライティングをする
	materialDataSphere->enableLighting = true;
	materialDataSphere->color = color;

	//書き込むためのアドレスを取得
	transformationMatrixResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere));
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
	transformationMatrixDataSphere->WVP = Multiply(worldMatrixSphere, ViewMatrix);
	dir_->GetCommandList_()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	transformationMatrixDataSphere->World = MakeIdentity4x4();

	dir_->GetCommandList_()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//色用のCBufferの場所を特定
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//頂点
	dir_->GetCommandList_()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere);
	//WVP
	dir_->GetCommandList_()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere->GetGPUVirtualAddress());
	dir_->GetCommandList_()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU : textureSrvHandleGPU2);
	dir_->GetCommandList_()->DrawInstanced(kSubdivision * kSubdivision * 6, 1, 0, 0);

	ImGui::Begin("Sphere");
	ImGui::SliderFloat3("ScaleSphere", &transformSphere.scale.x, 0.1f, 30, "%.3f");
	ImGui::SliderFloat3("RotateSphere", &transformSphere.rotate.x, -7, 7, "%.3f");
	ImGui::SliderFloat3("TranslateSphere",&transformSphere.translate.x, -10, 10, "%.3f");
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	ImGui::End();
}
void Triangle::MakeVertexBufferViewSphere()
{
	//リソースの先頭のアドレス
	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	//使用する頂点サイズ
	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision * kSubdivision;
	//1頂点あたりのアドレス
	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);
}

void Triangle::Release() {
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();

	vertexResourceSprite->Release();
	transformationMatrixResourceSprite->Release();
	materialResourceSprite->Release();

	vertexResourceSphere->Release();
	transformationMatrixResourceSphere->Release();
	materialResourceSphere->Release();

	directionalLightResource->Release();

}


void Triangle::LoadTexture() {

	// Textureを読んで転送する(1枚目)
	DirectX::ScratchImage mipImages = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(dir_->GetDevice(), metadata);
	UploadTextureData(textureResource, mipImages);

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
	dir_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

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
	ID3D12Resource* textureResoure2 = CreateTextureResource(dir_->GetDevice(), metadata2);
	UploadTextureData(textureResoure2, mipImages2);

	//meataDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
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

ID3D12Resource* Triangle::CreateBufferResource( size_t sizeInbytes) {
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
	hr_ = dir_->GetDevice()->CreateCommittedResource(
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


