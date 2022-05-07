#pragma once
#include "FrameWork.h"

namespace CoreEngine
{
	class C_Texture : public C_FrameWork
	{
	public:
		C_Texture();
		virtual ~C_Texture();

#ifdef UNICODE
	protected:
		LPCWSTR	m_pFileName;			// 파일 이름

	public:
		HRESULT	Init(LPCWSTR sFile);

#else
	protected:
		LPCSTR	m_pFileName;			// 파일 이름

	public:
		HRESULT	Init(LPCSTR sFile);

#endif // UNICODE


	protected:
		D3DXIMAGE_INFO		m_Img;		// 이미지 정보
		LPDIRECT3DTEXTURE9	m_pTex;		// 텍스처

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	public:
		INT		GetImageWidth();
		INT		GetImageHeight();
		void	GetImageRect(RECT* pRc);

		LPDIRECT3DTEXTURE9	GetTexture();
	};
}