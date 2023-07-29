#pragma once
#include <d3d12.h>
#include <cassert>

ID3D12DescriptorHeap* CreatDescriptorHeap(
	ID3D12Device* device, 
	D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors, 
	bool shaderVisible
);
