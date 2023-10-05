#include "Triangle.h"

void Triangle::Initialize(DirectXCommon* dir_, Vector4* pos) {

	Triangle::CreateVertexResource(dir_, pos);
	Triangle::CreateMaterialResource(dir_);
	Triangle::CreateWVPResource(dir_);
}

void Triangle::Update() {
}

void Triangle::Draw(DirectXCommon* dir_) {
	dir_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	dir_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	dir_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	dir_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dir_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	dir_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::Release() {
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
	textureResource->Release();
}

void Triangle::CreateVertexResource(DirectXCommon* dir_, Vector4* pos) {
	// 頂点用のリソースを作る。今回はcolor1つ分のサイズを用意する
	vertexResource = CreateBufferResource(dir_->GetDevice(), sizeof(VertexData) * 3);
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//// 左下
	//vertexData[0] = { -0.1f, -0.1f, 0.0f, 1.0f };
	//// 上
	//vertexData[1] = { 0.0f, 0.1f, 0.0f, 1.0f };
	//// 右上
	//vertexData[2] = { 0.1f, -0.1f, 0.0f, 1.0f };

	// 左下
	vertexData[0].position = pos[0];
	vertexData[0].texcoord = { 0.0f, 1.0f };
	// 上
	vertexData[1].position = pos[1];
	vertexData[1].texcoord = { 0.5f, 0.0f };

	// 右上
	vertexData[2].position = pos[2];
	vertexData[2].texcoord = { 1.0f, 1.0f };

	// Textureを読んで転送する
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
	textureSrvHandleCPU = dir_->srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dir_->srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	// 先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dir_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// SRVの生成
	dir_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
}

void Triangle::CreateMaterialResource(DirectXCommon* dir_) {
	//// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = CreateBufferResource(dir_->GetDevice(), sizeof(Vector4));

	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	materialBufferView.BufferLocation = materialResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	materialBufferView.SizeInBytes = sizeof(Vector4);
	// 1頂点あたりのサイズ
	materialBufferView.StrideInBytes = sizeof(Vector4);

	// マテリアルにデータを書き込む

	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 今回は赤を書き込んでみる
	*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Triangle::CreateWVPResource(DirectXCommon* dir_) {
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = CreateBufferResource(dir_->GetDevice(), sizeof(Matrix4x4));
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
