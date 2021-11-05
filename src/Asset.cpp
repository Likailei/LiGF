#include "Asset.h"

void Asset::LoadGLTF(const std::string fileName)
{
    m_gltfScene = fx::gltf::LoadFromText(fileName);
}

void Asset::UploadAssetsResource(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW* vBufferView, D3D12_INDEX_BUFFER_VIEW* iBufferView)
{
    /*auto sizes = CalculateMeshesTotalSize();
    UINT32 totalBufferSize = sizes.first + sizes.second;
    
    char str[30];
    sprintf_s(str, "TotalSize: %u\n", totalBufferSize);
    OutputDebugStringA(str);

    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(m_defaultResource.ReleaseAndGetAddressOf())));
    
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_uploadResource.ReleaseAndGetAddressOf())));

    UINT8* bufferStart{};
    const CD3DX12_RANGE range(0, 0);
    ThrowIfFailed(m_uploadResource->Map(0, &range, reinterpret_cast<void**>(&bufferStart)));

    for (auto const& mesh : m_gltfScene.meshes) {
        for (auto const& primitive : mesh.primitives) {
            m_defaultResources.resize(mesh.primitives.size());
            m_uploadResources.resize(mesh.primitives.size());

            for (size_t i=0; i<primitive.attributes.size(); i++)
            {
                auto& attrib = primitive.attributes;
                if (attrib.first == "POSITION")
                {
                    m_vertexBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
                }
                else if (attrib.first == "NORMAL")
                {
                    m_normalBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
                }
                else if (attrib.first == "TANGENT")
                {
                    m_tangentBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
                }
                else if (attrib.first == "TEXCOORD_0")
                {
                    m_texCoord0Buffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
                }
                m_indexBuffer = GetData(m_gltfScene, m_gltfScene.accessors[primitive.indices]);

                const size_t totalBufferSize =
                    static_cast<std::size_t>(m_vertexBuffer.TotalSize) +
                    static_cast<std::size_t>(m_normalBuffer.TotalSize) +
                    static_cast<std::size_t>(m_tangentBuffer.TotalSize) +
                    static_cast<std::size_t>(m_texCoord0Buffer.TotalSize) +
                    static_cast<std::size_t>(m_indexBuffer.TotalSize);

                ThrowIfFailed(device->CreateCommittedResource(
                    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                    D3D12_HEAP_FLAG_NONE,
                    &CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize),
                    D3D12_RESOURCE_STATE_COPY_DEST,
                    nullptr,
                    IID_PPV_ARGS(m_defaultResources[i]));
            }
        }
    }*/
}

Asset::BufferInfo Asset::GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor)
{
    fx::gltf::BufferView const& bufferView = doc.bufferViews[accessor.bufferView];
    fx::gltf::Buffer const& buffer = doc.buffers[bufferView.buffer];

    const UINT32 dataTypeSize = Asset::CalculateDataTypeSize(accessor);
    return Asset::BufferInfo{ &accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset], dataTypeSize, accessor.count * dataTypeSize };
}

void Asset::GetVertexBufferLayout()
{
    const auto& meshs = m_gltfScene.meshes;
    for (const auto& m : meshs) {
        for (const auto& p : m.primitives) {
            for (const auto& attrib : p.attributes) {
                const auto& acc = m_gltfScene.accessors[attrib.second];
                const auto& bv = m_gltfScene.bufferViews[acc.bufferView];
                auto offset = bv.byteOffset + acc.byteOffset; // from buffer0.start BUFFER1 2...
                auto size = bv.byteLength;
                auto stride = bv.byteStride;

                if (attrib.first == "POSITION") m_vertexLayout.push_back(VertexPartInfo{ "POSITION", stride, size, offset });
                else if (attrib.first == "NORMAL") m_vertexLayout.push_back(VertexPartInfo{ "NORMAL", stride, size, offset });
                //else if (attrib.first == "TANGENT") m_vertexLayout.push_back(VertexPartInfo{ "TANGENT", stride, size, offset });
                //else if (attrib.first == "TEXCOORD_0") m_vertexLayout.push_back(VertexPartInfo{ "TEXCOORD_0", stride, size, offset });
                //else if (attrib.first == "COLOR_0") m_vertexLayout.push_back(VertexPartInfo{ "COLOR_0", stride, size, offset });
            }
            std::sort(m_vertexLayout.begin(), m_vertexLayout.end(), [](const VertexPartInfo& a, const VertexPartInfo& b){return a.offset<b.offset;});
            
            const auto& acc = m_gltfScene.accessors[p.indices];
            const auto& bv = m_gltfScene.bufferViews[acc.bufferView];
            IndexPartInfo ipi{};
            ipi.offset = acc.byteOffset + bv.byteOffset;
            ipi.size = bv.byteLength;
            ipi.format = DXGI_FORMAT_R16_UINT;
            m_indexLayout.push_back(ipi);
        }
    }
}

void Asset::DumpInputElementDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputDesc)
{
    GetVertexBufferLayout();
    for (const auto& v : m_vertexLayout) {
        D3D12_INPUT_ELEMENT_DESC ied{};
        ied.SemanticName = v.name.c_str();
        ied.SemanticIndex = 0;
        ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        ied.InputSlot = 0;
        ied.AlignedByteOffset = v.offset;
        ied.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        ied.InstanceDataStepRate = 0;
        inputDesc.push_back(ied);
    }
}

void Asset::UploadOnce(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VERTEX_BUFFER_VIEW>& vBufferViews, std::vector<D3D12_INDEX_BUFFER_VIEW>& iBufferViews)
{
    UINT32 bufferSize = m_gltfScene.buffers[0].byteLength;
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(m_defaultResource.ReleaseAndGetAddressOf())));

    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_uploadResource.ReleaseAndGetAddressOf())));

    UINT8* bufferStart{};
    const CD3DX12_RANGE range(0, 0);
    ThrowIfFailed(m_uploadResource->Map(0, &range, reinterpret_cast<void**>(&bufferStart)));
    commandList->CopyBufferRegion(m_defaultResource.Get(), 0, m_uploadResource.Get(), 0, bufferSize);
    const CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER);
    commandList->ResourceBarrier(1, &barrier);

    D3D12_VERTEX_BUFFER_VIEW vbv{};
    vbv.BufferLocation = m_defaultResource->GetGPUVirtualAddress() + m_vertexLayout[0].offset;
    const auto& last = m_vertexLayout.back();
    vbv.SizeInBytes =last.offset + last.size;
    vbv.StrideInBytes = last.offset + last.stride;
    vBufferViews.push_back(vbv);

    D3D12_INDEX_BUFFER_VIEW ibv{};
    ibv.BufferLocation = m_defaultResource->GetGPUVirtualAddress() + m_indexLayout.begin()->offset;
    ibv.Format = m_indexLayout.begin()->format;
    ibv.SizeInBytes = m_indexLayout.begin()->size;
    iBufferViews.push_back(ibv);
}

std::string Asset::GetFileExtension(const std::string fileName)
{
    if (fileName.find_last_of(".") != std::string::npos)
        return fileName.substr(fileName.find_last_of(".") + 1);
    return "";
}

UINT32 Asset::CalculateDataTypeSize(fx::gltf::Accessor const& accessor) noexcept
{
    UINT32 elementSize = 0;
    switch (accessor.componentType)
    {
    case fx::gltf::Accessor::ComponentType::Byte:
    case fx::gltf::Accessor::ComponentType::UnsignedByte:
        elementSize = 1;
        break;
    case fx::gltf::Accessor::ComponentType::Short:
    case fx::gltf::Accessor::ComponentType::UnsignedShort:
        elementSize = 2;
        break;
    case fx::gltf::Accessor::ComponentType::Float:
    case fx::gltf::Accessor::ComponentType::UnsignedInt:
        elementSize = 4;
        break;
    }

    switch (accessor.type)
    {
    case fx::gltf::Accessor::Type::Mat2:
        return 4 * elementSize;
        break;
    case fx::gltf::Accessor::Type::Mat3:
        return 9 * elementSize;
        break;
    case fx::gltf::Accessor::Type::Mat4:
        return 16 * elementSize;
        break;
    case fx::gltf::Accessor::Type::Scalar:
        return elementSize;
        break;
    case fx::gltf::Accessor::Type::Vec2:
        return 2 * elementSize;
        break;
    case fx::gltf::Accessor::Type::Vec3:
        return 3 * elementSize;
        break;
    case fx::gltf::Accessor::Type::Vec4:
        return 4 * elementSize;
        break;
    }

    return 0;
}

UINT32 Asset::InitSingleMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW* vBufferView, D3D12_INDEX_BUFFER_VIEW* iBufferView, UINT32 meshIndex)
{
    const fx::gltf::Mesh& m = m_gltfScene.meshes[meshIndex];
    const fx::gltf::Attributes attribs = m.primitives[0].attributes;
    for (const auto& attrib : attribs) {
        if (attrib.first == "POSITION") {
            m_vertexBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
        }
        else if (attrib.first == "NORMAL")
        {
            m_normalBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
        }
        else if (attrib.first == "TANGENT")
        {
            m_tangentBuffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
        }
        else if (attrib.first == "TEXCOORD_0")
        {
            m_texCoord0Buffer = GetData(m_gltfScene, m_gltfScene.accessors[attrib.second]);
        }
    }
    m_indexBuffer = GetData(m_gltfScene, m_gltfScene.accessors[m.primitives[0].indices]);

    const size_t totalBufferSize =
        static_cast<std::size_t>(m_vertexBuffer.TotalSize) +
        static_cast<std::size_t>(m_normalBuffer.TotalSize) +
        static_cast<std::size_t>(m_tangentBuffer.TotalSize) +
        static_cast<std::size_t>(m_texCoord0Buffer.TotalSize) +
        static_cast<std::size_t>(m_indexBuffer.TotalSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(m_defaultResource.ReleaseAndGetAddressOf())));

    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_uploadResource.ReleaseAndGetAddressOf())));

    UINT8* bufferStart{};
    const CD3DX12_RANGE range(0, 0);
    ThrowIfFailed(m_uploadResource->Map(0, &range, reinterpret_cast<void**>(&bufferStart)));

    UINT32 offset{};
    memcpy(bufferStart, m_vertexBuffer.Data, m_vertexBuffer.TotalSize);
    vBufferView->BufferLocation = m_defaultResource->GetGPUVirtualAddress();
    vBufferView->SizeInBytes = m_vertexBuffer.TotalSize;
    vBufferView->StrideInBytes = m_vertexBuffer.DataStride;
    offset += vBufferView->SizeInBytes;

    // Normal...tangent...texcoord...
    
    memcpy(bufferStart+offset, m_indexBuffer.Data, m_indexBuffer.TotalSize);
    iBufferView->BufferLocation = m_defaultResource->GetGPUVirtualAddress()+offset;
    iBufferView->SizeInBytes = m_indexBuffer.TotalSize;
    iBufferView->Format = DXGI_FORMAT_R16_UINT;
    offset += iBufferView->SizeInBytes;

    commandList->CopyBufferRegion(m_defaultResource.Get(), 0, m_uploadResource.Get(), 0, totalBufferSize);
    const CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER);
    commandList->ResourceBarrier(1, &barrier);

    m_uploadResource->Unmap(0, nullptr);

    return  m_gltfScene.accessors[m.primitives[0].indices].count;
}

std::pair<UINT32,UINT32> Asset::CalculateMeshesTotalSize()
{
    UINT32 vertexSize{ 0 };
    UINT32 indexSize{ 0 };

    for (auto const& mesh : m_gltfScene.meshes) {
        for (auto const& primitive : mesh.primitives) {
            indexSize += CalculateDataTypeSize(m_gltfScene.accessors[primitive.indices]);
            for (auto const& attrib : primitive.attributes) {
                vertexSize += CalculateDataTypeSize(m_gltfScene.accessors[attrib.second]);
            }
        }
    }
    return std::make_pair(vertexSize, indexSize);
}
