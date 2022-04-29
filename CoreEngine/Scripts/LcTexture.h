#pragma once

class CLcTexture
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;	// Device

	D3DXIMAGE_INFO		m_Img;	// 이미지 정보
	LPDIRECT3DTEXTURE9	m_pTx;	// 텍스처

public:
	CLcTexture();
	virtual ~CLcTexture();

	virtual HRESULT	Create(LPDIRECT3DDEVICE9 pDev, LPCWSTR sFile);
	virtual void	Destroy();

public:
	INT		GetImageWidth();
	INT		GetImageHeight();
	void	GetImageRect(RECT* pRc);

	LPDIRECT3DTEXTURE9	GetTexture();
};
