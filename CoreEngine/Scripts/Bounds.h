#pragma once

namespace CoreEngine
{
	struct S_Bounds
	{
	private:
		D3DXVECTOR3* m_pSize;
		D3DXVECTOR3* m_pCenter;

	public:
		PROPERTY(D3DXVECTOR3, extends);
		GET(extends) { return (*m_pSize) * 0.5f; }
		SET(extends) { (*m_pSize) = _value * 2.0f; }

		PROPERTY(D3DXVECTOR3, size);
		GET(size) { return (*m_pSize); }
		SET(size) { (*m_pSize) = _value; }

		PROPERTY(D3DXVECTOR3, center);
		GET(center) { return (*m_pCenter); }
		SET(center) { (*m_pCenter) = _value; }

		PROPERTY(D3DXVECTOR3, min);
		GET(min) { return (*m_pCenter) - extends;/*+ D3DXVECTOR3(-extends.x, extends.y, 0.0f);*/ }
		SET(min)
		{
			(*m_pCenter) = max - _value;
			(*m_pSize) = ((*m_pCenter) - _value) * 2.0f;
		}

		PROPERTY(D3DXVECTOR3, max);
		GET(max) { return (*m_pCenter) + extends;/*+ D3DXVECTOR3(extends.x, -extends.y, 0.0f);*/ }
		SET(max)
		{
			(*m_pCenter) = max + _value;
			(*m_pSize) = (_value - (*m_pCenter)) * 2.0f;
		}

	public:
		void SetSize(D3DXVECTOR3* _size) { SAFE_DELETE(m_pSize); m_pSize = _size; }
		void SetCenter(D3DXVECTOR3* _center) { SAFE_DELETE(m_pCenter); m_pCenter = _center; }

	public:
		S_Bounds();
		~S_Bounds();

	};
}