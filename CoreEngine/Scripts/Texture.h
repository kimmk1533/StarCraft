#pragma once
#include "FrameWork.h"

namespace CoreEngine
{
	class C_Texture : public C_FrameWork
	{
	public:
		C_Texture();
		virtual ~C_Texture();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;
		
	public:
#ifdef UNICODE
		HRESULT	Init(LPCWSTR sFile);
#else
		HRESULT	Init(LPCSTR sFile);
#endif // UNICODE

	protected:
#ifdef UNICODE
		LPCWSTR	m_pFileName;			// 파일 이름
#else
		LPCSTR	m_pFileName;			// 파일 이름
#endif // UNICODE

		D3DXIMAGE_INFO		m_Img;		// 이미지 정보
		LPD3DXTEXTURE9	m_pTex;		// 텍스처

	public:
		INT		GetImageWidth();
		INT		GetImageHeight();
		void	GetImageRect(RECT* pRc);

		LPD3DXTEXTURE9	GetTexture();
		LPD3DXTEXTURE9	GetTexture() const;

	};
}