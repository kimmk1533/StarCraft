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
		HRESULT	Init(const std::string& _file);
		HRESULT	Init(const std::wstring& _file);

	protected:
		std::wstring m_FileName;		// 파일 이름

		D3DXIMAGE_INFO	m_Img;		// 이미지 정보
		LPD3DXTEXTURE9	m_pTex;		// 텍스처

	public:
		INT		GetImageWidth();
		INT		GetImageHeight();
		void	GetImageRect(RECT* pRc);

		LPD3DXTEXTURE9	GetTexture();
		LPD3DXTEXTURE9	GetTexture() const;

	};
}