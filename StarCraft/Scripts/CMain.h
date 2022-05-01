#pragma once

class C_Marine;

class C_Main : public C_Engine
{
private:
	C_Marine* m_Marine;

public:
	// Window+Device 관련 함수들
	virtual HRESULT	Create(HINSTANCE hInst) override;

public:
	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float deltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	Destroy() override;

};