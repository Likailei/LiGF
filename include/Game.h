#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "stdafx.h"
#include "Timer.h"
#include "Image.h"
#include "Font.h"
#include "Input.h"
#include "ThirdPersonCamera.h"
#include "Asset.h"
#include "Noise.h"
#include "Map.h"

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
    
	void OnKeyUp(UINT8 key);
    void OnMouseMove(WPARAM btnState, int x, int y);
    void OnMWheelRotate(short delta);
    void OnInput(LPARAM lParam);

	Image* m_imageMgr;
	Font* m_fontMgr;
    Input* m_inputMgr;
    Asset* m_assetMgr;

    std::vector<UINT32> m_indexCounts;
    std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vBufferViews;
    std::vector<D3D12_INDEX_BUFFER_VIEW> m_iBufferViews;
    Mesh ChunkMesh;
private:
    static const UINT FrameCount = 3;
    float d = -20.f;
    UINT m_width;
    UINT m_height;
    float m_aspectRatio;
    HWND m_hwnd;

    static const UINT TextureWidth = 512;
    static const UINT TextureHeight = 512;
    static const UINT8 TexturePixelSize{ 1 };

    

    ThirdPersonCamera m_camera;

    ConstBufferObject m_constBuffer;

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

    UINT8* m_constBufferGPUAddress[FrameCount];
    ComPtr<ID3D12Resource> m_constBufferUploadHeaps[FrameCount];
    ComPtr<ID3D12Resource> textureUploadHeap;

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
    std::vector<UINT8>& LoadTextureFromImg(const wchar_t* fileName);
};
#endif // !_GAME_H_
