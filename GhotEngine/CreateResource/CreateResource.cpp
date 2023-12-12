#include "CreateResource.h"

// Resource作成
Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource::CreateBufferResource(size_t sizeInBytes) {

	Microsoft::WRL::ComPtr <ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。	
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	HRESULT hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}

// IBV
D3D12_INDEX_BUFFER_VIEW CreateResource::CreateIndexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes) {

	// 頂点バッファビューを作成する
	D3D12_INDEX_BUFFER_VIEW IndexBufferView{};
	// リソースの先頭のアドレスから使う
	IndexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	IndexBufferView.SizeInBytes = uint32_t(sizeInBytes);
	// uint32_tを使う
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;

	return IndexBufferView;
}

// VBV
D3D12_VERTEX_BUFFER_VIEW CreateResource::CreateVertexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes, int size) {


	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// VertexBufferView
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeInBytes);
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = UINT(sizeInBytes / size);

	return vertexBufferView;
}