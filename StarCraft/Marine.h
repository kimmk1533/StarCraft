#pragma once

class CMarine : public CUnit
{
private:
	std::shared_ptr<CLcTexture*> m_pTex;

public:
	CMarine();
	virtual ~CMarine() override;

	virtual HRESULT	Create() override;
	virtual HRESULT	Create(LPDIRECT3DDEVICE9 _pDev);
	virtual HRESULT	Update(const float _deltaTime) override;
	virtual HRESULT	Render(CLcSprite* _drawer) override;
	virtual void	Destroy() override;
	virtual CLcTexture* GetTexture() const override;
};