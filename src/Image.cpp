#include "Image.h"

Image::Image() {
	ThrowIfFailed(CoInitialize(nullptr));
	ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pImageFactory)));
}

Image::~Image()
{
	SAFE_RELEASE(m_pImageDecoder);
	SAFE_RELEASE(m_pImageFactory);
	SAFE_RELEASE(m_pImageEncoder);
	SAFE_RELEASE(m_pImageFrame);
	SAFE_RELEASE(m_pPropertyBag);
	SAFE_RELEASE(m_pStream);
}

void Image::InitDecoderFromFile(const wchar_t* fileName)
{
	ThrowIfFailed(m_pImageFactory->CreateDecoderFromFilename(fileName, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_pImageDecoder));
}

UINT Image::GetFrames()
{
	UINT frameCount;
	ThrowIfFailed(m_pImageDecoder->GetFrameCount(&frameCount));

	for (UINT i = 0; i < frameCount; i++) {
		IWICBitmapFrameDecode* frame;
		m_pImageDecoder->GetFrame(i, &frame);
		m_pFrames.push_back(frame);
	}
	/*WICPixelFormatGUID wpf;
	m_pFrames[0]->GetPixelFormat(&wpf);*/

	return frameCount;
}

void Image::SaveBufferAsPng(CharBufferInfo fbb, const wchar_t* fileName)
{
	ThrowIfFailed(m_pImageFactory->CreateStream(&m_pStream));
	ThrowIfFailed(m_pStream->InitializeFromFilename(fileName, GENERIC_WRITE));
	ThrowIfFailed(m_pImageFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &m_pImageEncoder));
	ThrowIfFailed(m_pImageEncoder->Initialize(m_pStream, WICBitmapEncoderNoCache));
	ThrowIfFailed(m_pImageEncoder->CreateNewFrame(&m_pImageFrame, 0));
	
	// TODO
	/*PROPBAG2 option = { 0 };
	option.pstrName = (LPOLESTR)L"InterlaceOption";
	VARIANT varValue;
	VariantInit(&varValue);
	varValue.vt = VT_BOOL;
	varValue.boolVal = true;
	ThrowIfFailed(m_pPropertyBag->Write(1, &option, &varValue));*/
	
	ThrowIfFailed(m_pImageFrame->Initialize(0));
	ThrowIfFailed(m_pImageFrame->SetSize(fbb.width, fbb.rows));
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat24bppBGR;
	ThrowIfFailed(m_pImageFrame->SetPixelFormat(&formatGUID));

	UINT cbStride = (fbb.width * 24 + 7) / 8;
	UINT cbBufferSize = fbb.rows * cbStride;
	BYTE* pbBuffer = new BYTE[cbBufferSize];
	if (pbBuffer != NULL)
	{
		for (UINT j = 0; j < fbb.rows; j++) {
			for (UINT i = 0; i < fbb.width; i++) {
				UINT dstBufferIdx = cbStride * j + i * 3;
				UINT srcBufferIdx = fbb.pitch * j + i;
				UINT8 pixel = fbb.buffer[srcBufferIdx];
				pbBuffer[dstBufferIdx] = pixel;
				pbBuffer[dstBufferIdx + 1] = pixel;
				pbBuffer[dstBufferIdx + 2] = pixel;
			}
		}
		m_pImageFrame->WritePixels(fbb.rows, cbStride, cbBufferSize, pbBuffer);
		delete[] pbBuffer;
	}
	m_pImageFrame->Commit();
	m_pImageEncoder->Commit();
}

void Image::GetPngBuffer(std::vector<UINT8>& buffer, const wchar_t* fileName, UINT* w, UINT* h)
{
	InitDecoderFromFile(fileName);
	GetFrames();
	m_pFrames[0]->GetSize(w, h);
	buffer.resize(*w * *h * 4);
	m_pFrames[0]->CopyPixels(nullptr, *w * 4, *w * *h * 4, buffer.data());
}
