#include "stdafx_Core.h"
#include "Camera.h"

namespace CoreEngine
{
	C_Camera::C_Camera()
	{
		m_pEye = nullptr;
		m_pLookAt = nullptr;
		m_pUp = nullptr;

		m_fCameraSpeed = 1.0f;

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
		m_pEye = new D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		m_pLookAt = new D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_pUp = new D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_fCameraSpeed = 1.0f;

		m_pU = new D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_pV = new D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_pW = new D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		m_pRMatrix = new D3DXMATRIX();
		m_pTMatrix = new D3DXMATRIX();
		m_pViewMatrix = new D3DXMATRIX();

		D3DXMatrixIdentity(m_pRMatrix);
		D3DXMatrixIdentity(m_pTMatrix);
		D3DXMatrixIdentity(m_pViewMatrix);

		m_pRMatrix->_11 = m_pU->x; m_pRMatrix->_12 = m_pV->x; m_pRMatrix->_13 = m_pW->x;
		m_pRMatrix->_21 = m_pU->y; m_pRMatrix->_22 = m_pV->y; m_pRMatrix->_23 = m_pW->y;
		m_pRMatrix->_31 = m_pU->z; m_pRMatrix->_32 = m_pV->z; m_pRMatrix->_33 = m_pW->z;

		m_pTMatrix->_41 = -(*m_pEye).x; m_pTMatrix->_42 = -(*m_pEye).y; m_pTMatrix->_43 = -(*m_pEye).z;

		D3DXMatrixMultiply(m_pViewMatrix, m_pTMatrix, m_pRMatrix);

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
			if (Input->GetKey(E_KeyCode::LeftArrow))
			{
				m_pLookAt->x = m_pEye->x -= m_fCameraSpeed;
			}
			if (Input->GetKey(E_KeyCode::RightArrow))
			{
				m_pLookAt->x = m_pEye->x += m_fCameraSpeed;
			}
			if (Input->GetKey(E_KeyCode::UpArrow))
			{
				m_pLookAt->y = m_pEye->y += m_fCameraSpeed;
			}
			if (Input->GetKey(E_KeyCode::DownArrow))
			{
				m_pLookAt->y = m_pEye->y -= m_fCameraSpeed;
			}

			//m_pEye->z += Input->GetMouseEps().z;
		}

		std::cout << *m_pEye << "\n";

		D3DXMatrixIdentity(m_pViewMatrix);

		m_pTMatrix->_41 = -(*m_pEye).x; m_pTMatrix->_42 = -(*m_pEye).y; m_pTMatrix->_43 = -(*m_pEye).z;

		D3DXMatrixMultiply(m_pViewMatrix, m_pTMatrix, m_pRMatrix);

		return S_OK;
	}
}