#pragma once

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <wincodec.h>
#include <vector>
#include "stdafx.h"

class Image {
public:
	Image();
	~Image();

	void InitDecoderFromFile(const wchar_t* fileName);
	UINT GetFrames();
	void SaveBufferAsPng(CharBufferInfo fbb, const wchar_t* fileName);

	std::vector<IWICBitmapFrameDecode*> m_pFrames;

private:
	IWICImagingFactory* m_pImageFactory;
	IWICBitmapDecoder* m_pImageDecoder;
	IWICStream* m_pStream;
	IWICBitmapEncoder* m_pImageEncoder;
	IWICBitmapFrameEncode* m_pImageFrame;
	IPropertyBag2* m_pPropertyBag;
	
};

#endif // !_IMAGE_H_

