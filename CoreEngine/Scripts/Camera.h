#pragma once

namespace CoreEngine
{
	class C_Camera : public IFrameWork, public IUpdatable
	{
	private:
		D3DXVECTOR3* m_pEye;
		D3DXVECTOR3* m_pLookAt;
		D3DXVECTOR3* m_pUp;

	public:
		READONLY_PROPERTY(D3DXVECTOR3, position);
		GET(position) { return *m_pEye; }

		READONLY_PROPERTY(D3DXVECTOR3, forward);
		GET(forward) { return *m_pLookAt; }

		READONLY_PROPERTY(D3DXVECTOR3, up);
		GET(up) { return *m_pUp; }

	private:
		FLOAT m_fCameraSpeed;

		// 참고: https://vitacpp.tistory.com/43?category=961147
	private:
		D3DXVECTOR3* m_pU; // 카메라의 로컬 x축
		D3DXVECTOR3* m_pV; // 카메라의 로컬 y축
		D3DXVECTOR3* m_pW; // 카메라의 로컬 z축

		D3DXMATRIX* m_pRMatrix;
		D3DXMATRIX* m_pTMatrix;
		D3DXMATRIX* m_pViewMatrix;

	public:
		READONLY_PROPERTY(const D3DXMATRIX*, viewMatrix);
		GET(viewMatrix) { return m_pViewMatrix; }

	public:
		D3DXVECTOR3 WorldToScreenPoint(const D3DXVECTOR3& _pos);

	public:
		C_Camera();
		~C_Camera();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime);

	};
}