#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "LcTexture.h"
#include "McUtil.h"
#include <string>

CLcTexture::CLcTexture()
{
	m_pDev = NULL;

	m_pTx = NULL;
	ZeroMemory(&m_Img, sizeof(m_Img));
}
CLcTexture::~CLcTexture()
{
	Destroy();
}

HRESULT CLcTexture::Create(LPDIRECT3DDEVICE9 pDev, LPCWSTR sFile)
{
	m_pDev = pDev;

	DWORD	dColorKey = 0x00FFFFFF;

	std::wstring sfile = L"Resources/" + std::wstring(sFile);

	if (FAILED
	(
		D3DXCreateTextureFromFileEx
		(
			m_pDev,				// 디바이스 포인터
			sfile.c_str(),		// 텍스처 파일 이름
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,					// 밉 레벨(2D에서는 반드시 1)
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,	// 필터링
			D3DX_FILTER_NONE,	// 밉 필터링
			dColorKey,			// 컬러 키
			&m_Img,				// 텍스처 인포메이션
			NULL,
			&m_pTx				// 텍스처 포인터
		)
	))
	{
		if (FAILED
		(
			D3DXCreateTextureFromFileEx
			(
				m_pDev,				// 디바이스 포인터
				sFile,				// 텍스처 파일 이름
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,					// 밉 레벨(2D에서는 반드시 1)
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,	// 필터링
				D3DX_FILTER_NONE,	// 밉 필터링
				dColorKey,			// 컬러 키
				&m_Img,				// 텍스처 인포메이션
				NULL,
				&m_pTx				// 텍스처 포인터
			)
		))
		{
			m_pTx = NULL;
			MessageBox(GetActiveWindow(),
				L"Create Texture Failed",
				L"Err",
				MB_ICONEXCLAMATION);

			return E_FAIL;
		}
	}

	return S_OK;
}
void CLcTexture::Destroy()
{
	SAFE_RELEASE(m_pTx);
}

INT CLcTexture::GetImageWidth()
{
	return m_Img.Width;
}
INT CLcTexture::GetImageHeight()
{
	return m_Img.Height;
}
void CLcTexture::GetImageRect(RECT* pRc)
{
	pRc->left = 0;
	pRc->top = 0;
	pRc->right = m_Img.Width;
	pRc->bottom = m_Img.Height;
}

LPDIRECT3DTEXTURE9 CLcTexture::GetTexture()
{
	return m_pTx;
}