#include"Triangle.h"
#include "ConvertString.h"
#include"DirectX12.h"
#include"ImguiManege.h"

Triangle::Triangle() {
	dxcCompiler_ = nullptr;
	includeHandler_ = nullptr;
	vertexData_;
	materialData_;
	viewport_;
	scissorRect_;
	vertexBufferView_;
	triangleData[10];
	wvpData_ = nullptr;
	wvpResource_ = nullptr;
	wvpmResource_ = nullptr;
	transform_={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransfrom_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	transformationMatrixData_ = nullptr;
	
}

Triangle::~Triangle() {

}

void Triangle::Init(DirectX12* dx12Common) {
	
	
	dx12Common_ = dx12Common;

	vertexResource_ = CreatBufferResource(sizeof(VertexData) * 3);
	materialResource_ = CreatBufferResource(sizeof(Vector4) * 3);
	wvpResource_ = CreatBufferResource(sizeof(Matrix4x4));
	MakeVertexBufferView();
}

void Triangle::Draw(Vector4 triangleData[10] ) {
	///---------------------------------------------------------------------//
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f, };
	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	///---------------------------------------------------------------------///

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = color;

	//WVP用のリソースを作る。matrix4x4 一つ分サイズ分を用意する
	//データを書き込むためのアドレスを取得
	Matrix4x4 WorldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	*wvpData_ = Multiply(WorldMatrix, ViewMatrix);

	/*Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransfrom_.scale, cameraTransfrom_.rotate, cameraTransfrom_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280 / 720, 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*transformationMatrixData_ = worldViewProjectionMatrix;*/


	ImGui::Begin("Player pos");
	ImGui::ColorEdit3("color", &color.x);
	ImGui::End();


	//コマンド積む
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	dx12Common_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
//	commandList_->IASetVertexBuffers(0, 1, &materialBufferView_);
	//形状を設定。PSOに設定しているものとはまたは別。同じものを設定すると考えておけばよい
	dx12Common_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx12Common_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dx12Common_->GetcommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dx12Common_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

	//描画！（DrawCall/ドローコール）。３ちょうてんで１つのインスタンス。インスタンスについては今後
	dx12Common_->GetcommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::Release() {

	vertexResource_->Release();
	materialResource_->Release();
	wvpResource_->Release();
	textureResource->Release();
	intermediateResource_->Release();
}

void Triangle::LoadTexture(const std::string& filePath) {
	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = ImageFileOpen(filePath);
	const DirectX::TexMetadata& metdata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(dx12Common_->Getdevice(), metdata);
	intermediateResource_ = UploadTextureData(textureResource, mipImages);
	//metDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metdata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metdata.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dx12Common_->GetsrvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dx12Common_->GetsrvDescriptorHeap_()->GetGPUDescriptorHandleForHeapStart();

	//先頭はImGuiが使っているのでその次に使う
	textureSrvHandleCPU.ptr += dx12Common_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dx12Common_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	dx12Common_->Getdevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);


}

DirectX::ScratchImage Triangle::ImageFileOpen(const std::string& filePath) {
	//テクスチャファイルを読み込んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	 hr_ = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr_));

	//ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr_ = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr_));

	//ミニマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* Triangle::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);//textureの幅
	resourceDesc.Height = UINT(metadata.height);//Textureの高さ
	resourceDesc.MipLevels = UINT(metadata.mipLevels);//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行き　or　配列textureの配列数
	resourceDesc.Format = metadata.format;//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//Textureの次元数。普段使っているのは2次元

	//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケースがある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;//細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置

	//Resoureの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}
ID3D12Resource* Triangle::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(dx12Common_->Getdevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = CreatBufferResource(intermediateSize);
	UpdateSubresources(dx12Common_->GetcommandList(), texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());
	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dx12Common_->GetcommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;

}

void Triangle::MakeVertexBufferView() {
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//リソースの先頭のアドレスは頂点３つぶんのサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

ID3D12Resource* Triangle::CreatBufferResource( size_t sizeInBytes) {
	ID3D12Resource* Resource_ = nullptr;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes;//リソースのサイズ。今回はVector4を３頂点分
	//バッファの場合はこれらは１にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれらにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	hr_ = dx12Common_->Getdevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource_));
	assert(SUCCEEDED(hr_));

	return Resource_;

}



