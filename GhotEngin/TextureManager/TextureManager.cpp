#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

uint32_t TextureManager::Load(const std::string& fileName) {


	TextureManager::GetInstance()->index_++;
	LoadTex(fileName, TextureManager::GetInstance()->index_);

	return TextureManager::GetInstance()->index_;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUHandle(uint32_t texHandle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = TextureManager::GetInstance()->gpuDescHandleSRV[texHandle];

	return handleGPU;
}

void TextureManager::Initialize() {

	size.SRV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

//void TextureManager::ResetAllTex(){
//
//}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミニマップ付きのデータを返す
	return mipImages;
}

void TextureManager::LoadTex(const std::string& filePath, uint32_t index)
{

	DirectX::ScratchImage mipImages = LoadTexture(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	TextureManager::GetInstance()->texResource[index] = CreateTextureResource(metadata);
	UploadTextureData(TextureManager::GetInstance()->texResource[index].Get(), mipImages);

	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める
	TextureManager::GetInstance()->cpuDescHandleSRV[index] = GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetSRV(), TextureManager::GetInstance()->size.SRV, index + 1);
	TextureManager::GetInstance()->gpuDescHandleSRV[index] = GetGPUDescriptorHandle(DirectXCommon::GetInstance()->GetSRV(), TextureManager::GetInstance()->size.SRV, index + 1);
	//先頭はImGuiが使ってるのでその次を使う
	/*TextureManager::GetInstance()->cpuDescHandleSRV[index].ptr += DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	TextureManager::GetInstance()->gpuDescHandleSRV[index].ptr += DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);*/
	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(TextureManager::GetInstance()->texResource[index].Get(), &srvDesc, TextureManager::GetInstance()->cpuDescHandleSRV[index]);

}

ID3D12Resource* TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {

	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width); // Textureの幅
	resourceDesc.Height = UINT(metadata.height); // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列Textureの配列数
	resourceDesc.Format = metadata.format; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);  // Textureの次元数。普段使っているのは2次元
	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置
	// Resourceの作成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ, // 初回のResourceState。Textureは基本読むだけ
		nullptr, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;

}

void TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {
	// Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// 全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		// MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr, // 全領域へコピー
			img->pixels, // 元データアドレス
			UINT(img->rowPitch), // 1ラインサイズ
			UINT(img->slicePitch) // 1枚サイズ
		);
		assert(SUCCEEDED(hr));
	}

}