#pragma once

namespace CoreEngine
{
	struct S_Bounds : public IFrameWork
	{
	private:
		std::shared_ptr<D3DXVECTOR3> m_pSize;
		std::shared_ptr<D3DXVECTOR3> m_pCenter;

	public:
		PROPERTY(D3DXVECTOR3, extends);
		GET(extends) { return (*m_pSize) * 0.5f; }
		SET(extends) { (*m_pSize) = _value * 2.0f; }
		PROPERTY(const D3DXVECTOR3, c_extends);
		const GET(c_extends) const { return (*m_pSize) * 0.5f; }
		const SET(c_extends) { (*m_pSize) = _value * 2.0f; }

		PROPERTY(D3DXVECTOR3, size);
		GET(size) { return (*m_pSize); }
		SET(size) { (*m_pSize) = _value; }
		PROPERTY(const D3DXVECTOR3, c_size);
		const GET(c_size) const { return (*m_pSize); }
		const SET(c_size) { (*m_pSize) = _value * 2.0f; }

		PROPERTY(D3DXVECTOR3, center);
		GET(center) { return (*m_pCenter); }
		SET(center) { (*m_pCenter) = _value; }
		PROPERTY(const D3DXVECTOR3, c_center);
		const GET(c_center) const { return (*m_pCenter); }
		const SET(c_center) { (*m_pCenter) = _value; }

		PROPERTY(D3DXVECTOR3, min);
		GET(min) { return (*m_pCenter) - extends;/*+ D3DXVECTOR3(-extends.x, extends.y, 0.0f);*/ }
		SET(min)
		{
			(*m_pCenter) = max - _value;
			(*m_pSize) = ((*m_pCenter) - _value) * 2.0f;
		}
		PROPERTY(const D3DXVECTOR3, c_min);
		const GET(c_min) const { return (*m_pCenter) - c_extends; }
		const SET(c_min)
		{
			(*m_pCenter) = c_max - _value;
			(*m_pSize) = ((*m_pCenter) - _value) * 2.0f;
		}

		PROPERTY(D3DXVECTOR3, max);
		GET(max) { return (*m_pCenter) + extends;/*+ D3DXVECTOR3(extends.x, -extends.y, 0.0f);*/ }
		SET(max)
		{
			(*m_pCenter) = max + _value;
			(*m_pSize) = (_value - (*m_pCenter)) * 2.0f;
		}
		PROPERTY(const D3DXVECTOR3, c_max);
		const GET(c_max) const { return (*m_pCenter) + c_extends; }
		const SET(c_max)
		{
			(*m_pCenter) = c_max + _value;
			(*m_pSize) = (_value - (*m_pCenter)) * 2.0f;
		}

	public:
		void SetSize(std::shared_ptr<D3DXVECTOR3> _size) { m_pSize = nullptr; m_pSize = _size; }
		void SetCenter(std::shared_ptr<D3DXVECTOR3> _center) { m_pCenter = nullptr; m_pCenter = _center; }

	public:
		S_Bounds();
		~S_Bounds();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	};

	std::ostream& operator<<(std::ostream& _out, const S_Bounds& _bounds);
}