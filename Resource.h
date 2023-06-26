#pragma once
#include <d3d12.h>
#include"Vector4.h"

ID3D12Resource* CreatBufferResource(ID3D12Device* device, size_t sizeInBytes);

//ID3D12Resource* vertexResource = CreatBufferResource(device, sizeof(Vector4) * 3);