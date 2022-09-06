#include "stdafx_Core.h"
#include "Camera.h"

namespace CoreEngine
{
	void C_Camera::Calculate_U_Vector()
	{
		static const D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXVec3Cross(m_pU, &up, m_pW);
		D3DXVec3Normalize(m_pU, m_pU);
	}
	void C_Camera::Calculate_V_Vector()
	{
		D3DXVec3Cross(m_pV, m_pW, m_pU);
	}
	void C_Camera::Calculate_W_Vector()
	{
		D3DXVec3Subtract(m_pW, m_pLookAt, m_pEye);
		D3DXVec3Normalize(m_pW, m_pW);
	}
	void C_Camera::Calculate_R_Matrix()
	{
		Calculate_W_Vector();
		Calculate_U_Vector();
		Calculate_V_Vector();

		D3DXMatrixIdentity(m_pRMatrix);

		m_pRMatrix->_11 = m_pU->x; m_pRMatrix->_12 = m_pV->x; m_pRMatrix->_13 = m_pW->x;
		m_pRMatrix->_21 = m_pU->y; m_pRMatrix->_22 = m_pV->y; m_pRMatrix->_23 = m_pW->y;
		m_pRMatrix->_31 = m_pU->z; m_pRMatrix->_32 = m_pV->z; m_pRMatrix->_33 = m_pW->z;
	}
	void C_Camera::Calculate_T_Matrix()
	{
		D3DXMatrixIdentity(m_pTMatrix);
		m_pTMatrix->_41 = -(*m_pEye).x; m_pTMatrix->_42 = (*m_pEye).y; m_pTMatrix->_43 = -(*m_pEye).z;
	}
	void C_Camera::Calculate_View_Matrix()
	{
		Calculate_R_Matrix();
		Calculate_T_Matrix();

		D3DXMatrixIdentity(m_pViewMatrix);
		D3DXMatrixMultiply(m_pViewMatrix, m_pTMatrix, m_pRMatrix);
	}

	void C_Camera::CameraCliping(const RECT& _rect)
	{
		if (m_pEye->x < _rect.left)		m_pEye->x = _rect.left;
		if (m_pEye->x > _rect.right)	m_pEye->x = _rect.right;
		if (m_pEye->y < _rect.bottom)	m_pEye->y = _rect.bottom;
		if (m_pEye->y > _rect.top)		m_pEye->y = _rect.top;

		if (m_pLookAt->x < _rect.left)		m_pLookAt->x = _rect.left;
		if (m_pLookAt->x > _rect.right)		m_pLookAt->x = _rect.right;
		if (m_pLookAt->y < _rect.bottom)	m_pLookAt->y = _rect.bottom;
		if (m_pLookAt->y > _rect.top)		m_pLookAt->y = _rect.top;
	}

	D3DXVECTOR3 C_Camera::WorldToScreenPoint(const D3DXVECTOR3& _pos)
	{
		return _pos + (*m_pEye + D3DXVECTOR3(0.0f, 0.0f, -m_pEye->z));
	}
	void C_Camera::SetClipingRect(const RECT& _rect)
	{
		m_ClipingRect = _rect;
	}

	void C_Camera::MoveCameraX(const FLOAT& _speed)
	{
		this->MoveCamera(D3DXVECTOR2(_speed, 0.0f));
	}
	void C_Camera::MoveCameraY(const FLOAT& _speed)
	{
		this->MoveCamera(D3DXVECTOR2(0.0f, _speed));
	}
	void C_Camera::MoveCamera(const D3DXVECTOR2& _move, const bool& _pixelPerfect)
	{
		if (_pixelPerfect)
		{
			static D3DXVECTOR2 speed = D3DXVECTOR2(0.0f, 0.0f);

			speed += _move;

			m_pLookAt->x = m_pEye->x += (int)speed.x;
			m_pLookAt->y = m_pEye->y += (int)speed.y;

			speed.x -= (int)speed.x;
			speed.y -= (int)speed.y;
		}
		else
		{
			m_pLookAt->x = m_pEye->x += _move.x;
			m_pLookAt->y = m_pEye->y += _move.y;
		}
	}

	C_Camera::C_Camera()
	{
		m_pEye = nullptr;
		m_pLookAt = nullptr;
		m_pUp = nullptr;

		m_CameraSpeed = 0.0f;
		m_ClipingRect = RECT{ 0, 0, 0, 0 };

		m_pU = nullptr;
		m_pV = nullptr;
		m_pW = nullptr;

		m_pRMatrix = nullptr;
		m_pTMatrix = nullptr;
		m_pViewMatrix = nullptr;
	}
	C_Camera::~C_Camera()
	{
		Destroy();
	}

	HRESULT C_Camera::Create()
	{
		m_pEye = new D3DXVECTOR3(0.0f, 600, -1.0f);
		m_pLookAt = new D3DXVECTOR3(0.0f, 600, 0.0f);
		m_pUp = new D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_CameraSpeed = 500.0f;
		SetClipingRect(C_Engine::GetScreenRect());

		m_pU = new D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_pV = new D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_pW = new D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		m_pRMatrix = new D3DXMATRIX();
		m_pTMatrix = new D3DXMATRIX();
		m_pViewMatrix = new D3DXMATRIX();

		Calculate_View_Matrix();

		return S_OK;
	}
	void C_Camera::Destroy()
	{
		SAFE_DELETE(m_pViewMatrix);
		SAFE_DELETE(m_pTMatrix);
		SAFE_DELETE(m_pRMatrix);

		SAFE_DELETE(m_pW);
		SAFE_DELETE(m_pV);
		SAFE_DELETE(m_pU);

		SAFE_DELETE(m_pUp);
		SAFE_DELETE(m_pLookAt);
		SAFE_DELETE(m_pEye);
	}

	HRESULT C_Camera::Update(const FLOAT& _deltaTime)
	{
		if (!Input->GetMouse(E_MouseCode::Left))
		{
			FLOAT speed = m_CameraSpeed * _deltaTime;

			if (Input->GetKey(E_KeyCode::LeftArrow))
			{
				this->MoveCameraX(-speed);
			}
			if (Input->GetKey(E_KeyCode::RightArrow))
			{
				this->MoveCameraX(speed);
			}
			if (Input->GetKey(E_KeyCode::UpArrow))
			{
				this->MoveCameraY(speed);
			}
			if (Input->GetKey(E_KeyCode::DownArrow))
			{
				this->MoveCameraY(-speed);
			}

			//m_pEye->z += Input->GetMouseEps().z;
		}

		//std::cout << *m_pEye << "\n";

		CameraCliping(m_ClipingRect);

		Calculate_View_Matrix();

		return S_OK;
	}
}