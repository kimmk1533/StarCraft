#include "stdafx_Core.h"

namespace CoreEngine
{
	C_Texture::C_Texture()
	{
		m_pTex = NULL;
		ZeroMemory(&m_Img, sizeof(m_Img));
	}
	C_Texture::~C_Texture()
	{
		Destroy();
	}

	HRESULT C_Texture::Create()
	{
		DWORD	dColorKey = 0x00FFFFFF;

		std::wstring file = TEXT("Resources/") + std::wstring(m_pFileName);

		if (FAILED
		(
			D3DXCreateTextureFromFileEx
			(
				m_pd3dDevice,			// 디바이스 포인터
				file.c_str(),			// 텍스처 파일 이름
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,						// 밉 레벨(2D에서는 반드시 1)
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,		// 필터링
				D3DX_FILTER_NONE,		// 밉 필터링
				dColorKey,				// 컬러 키
				&m_Img,					// 텍스처 인포메이션
				NULL,
				&m_pTex					// 텍스처 포인터
			)
		))
		{
			if (FAILED
			(
				D3DXCreateTextureFromFileEx
				(
					m_pd3dDevice,			// 디바이스 포인터
					m_pFileName,			// 텍스처 파일 이름
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					1,						// 밉 레벨(2D에서는 반드시 1)
					0,
					D3DFMT_UNKNOWN,
					D3DPOOL_MANAGED,
					D3DX_FILTER_NONE,		// 필터링
					D3DX_FILTER_NONE,		// 밉 필터링
					dColorKey,				// 컬러 키
					&m_Img,					// 텍스처 인포메이션
					NULL,
					&m_pTex					// 텍스처 포인터
				)
			))
			{
				m_pTex = NULL;
				MessageBox(GetActiveWindow(),
					TEXT("Texture::Create: 텍스쳐 파일이 존재하지 않습니다."),
					TEXT("Error!"),
					MB_ICONEXCLAMATION);

				return E_FAIL;
			}
		}

		return S_OK;
	}
	void C_Texture::Destroy()
	{
		SAFE_RELEASE(m_pTex);
	}

#ifdef UNICODE
	HRESULT C_Texture::Init(LPCWSTR sFile)
	{
		m_pFileName = sFile;

		return S_OK;
	}
#else
	HRESULT C_Texture::Init(LPCSTR sFile)
	{
		m_pFileName = sFile;

		return S_OK;
	}
#endif // UNICODE
	INT C_Texture::GetImageWidth()
	{
		return m_Img.Width;
	}
	INT C_Texture::GetImageHeight()
	{
		return m_Img.Height;
	}
	void C_Texture::GetImageRect(RECT* pRc)
	{
		pRc->left = 0;
		pRc->top = 0;
		pRc->right = m_Img.Width;
		pRc->bottom = m_Img.Height;
	}

	LPD3DXTEXTURE9 C_Texture::GetTexture()
	{
		return m_pTex;
	}
	LPD3DXTEXTURE9 C_Texture::GetTexture() const
	{
		return m_pTex;
	}
}