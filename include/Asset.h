#pragma once
#ifndef _ASSET_H_
#define _ASSET_H_

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "stdafx.h"
#include "gltf.h"

using Microsoft::WRL::ComPtr;

class Asset
{
public:
    struct BufferInfo
    {
        fx::gltf::Accessor const* Accessor;

        uint8_t const* Data;
        UINT32 DataStride;
        UINT32 TotalSize;
        UINT IndexCount;

        bool HasData() const noexcept
        {
            return Data != nullptr;
        }
    };

    struct VertexPartInfo {
        std::string name;
        UINT stride;
        UINT32 size;
        UINT32 offset;
    };
    
    struct IndexPartInfo {
        UINT32 offset;
        UINT32 size;
        DXGI_FORMAT format;
    };

    std::vector<VertexPartInfo> m_vertexLayout;
    std::vector<IndexPartInfo> m_indexLayout;

	void LoadGLTF(const std::string fileName);
	void UploadAssetsResource(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW* vBufferView, D3D12_INDEX_BUFFER_VIEW* iBufferView);

	static UINT32 CalculateDataTypeSize(fx::gltf::Accessor const& accessor) noexcept;
    UINT32 InitSingleMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW* vBufferView, D3D12_INDEX_BUFFER_VIEW* iBufferView, UINT32 meshIndex);

    void UploadOnce(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VERTEX_BUFFER_VIEW>& vBufferViews, std::vector<D3D12_INDEX_BUFFER_VIEW>& iBufferViews);
    void DumpInputElementDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputDesc);
private:
	static std::string GetFileExtension(const std::string fileName);
    BufferInfo GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor);
    std::pair<UINT32, UINT32> CalculateMeshesTotalSize();
    void GetVertexBufferLayout();

    fx::gltf::Document m_gltfScene{};
    BufferInfo m_indexBuffer{};
    BufferInfo m_vertexBuffer{};
    BufferInfo m_normalBuffer{};
    BufferInfo m_tangentBuffer{};
    BufferInfo m_texCoord0Buffer{};

    ComPtr<ID3D12Resource> m_defaultResource;
    ComPtr<ID3D12Resource> m_uploadResource;
};
#endif

