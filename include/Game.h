#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "stdafx.h"
#include "Timer.h"
#include "Image.h"
#include "Font.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Game {
public:
	Game(UINT width, UINT height, std::wstring name, HWND hwnd);
	~Game();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
	//virtual void OnKeyDown(UINT8 key);
	//virtual void OnKeyUp(UINT8 key);

	Image* m_imageMgr;
	Font* m_fontMgr;

private:
    static const UINT FrameCount = 3;
    
    UINT m_width;
    UINT m_height;
    float m_aspectRatio;
    HWND m_hwnd;

    static const UINT TextureWidth = 256;
    static const UINT TextureHeight = 256;
    static const UINT8 TexturePixelSize{ 4 };

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT2 uv;
    };

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12Resource> m_depthStencil;
    ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature >m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
    ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
    ComPtr<ID3D12DescriptorHeap> m_samplerHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    UINT m_numIndices;
    ComPtr<ID3D12Resource> m_vertexBuffer;
    ComPtr<ID3D12Resource> m_indexBuffer;
    ComPtr<ID3D12Resource> m_texture;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    UINT m_cbvSrvDescriptorSize;
    UINT m_rtvDescriptorSize;

    UINT m_frameIndex;
    UINT m_frameCounter;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValues[FrameCount];

    void LoadPipeline();
    void LoadAssets();
    void CreateFrameResources();
    void PopulateCommandList();
    void WaitForGpu();
    void MoveToNextFrame();
    CharBufferInfo GetCharTexture(wchar_t ch, UINT size);
};
#endif // !_GAME_H_
