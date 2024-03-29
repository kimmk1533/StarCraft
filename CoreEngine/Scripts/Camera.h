#pragma once

namespace CoreEngine
{
	class C_Camera : public IFrameWork, public IUpdatable
	{
	private:
		D3DXVECTOR3* m_pEye;
		D3DXVECTOR3* m_pLookAt;
		D3DXVECTOR3* m_pUp;

		FLOAT m_CameraSpeed;
		RECT m_ClipingRect;

	public:
		READONLY_PROPERTY(D3DXVECTOR3, position);
		GET(position) { return *m_pEye; }

		READONLY_PROPERTY(D3DXVECTOR3, forward);
		GET(forward) { return *m_pLookAt; }

		READONLY_PROPERTY(D3DXVECTOR3, up);
		GET(up) { return *m_pUp; }

		PROPERTY(FLOAT, speed);
		GET(speed) { return m_CameraSpeed; }
		SET(speed) { m_CameraSpeed = _value; }

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

	private:
		void Calculate_U_Vector();
		void Calculate_V_Vector();
		void Calculate_W_Vector();
		void Calculate_R_Matrix();
		void Calculate_T_Matrix();
		void Calculate_View_Matrix();

		void CameraCliping(const RECT& _rect);

	public:
		D3DXVECTOR3 WorldToScreenPoint(const D3DXVECTOR3& _pos);
		void SetClipingRect(const RECT& _rect);

		void MoveCameraX(const FLOAT& _speed);
		void MoveCameraY(const FLOAT& _speed);
		void MoveCamera(const D3DXVECTOR2& _move, const bool& _pixelPerfect = true);

	public:
		C_Camera();
		~C_Camera();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime);

	};
}