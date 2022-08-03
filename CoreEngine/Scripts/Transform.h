#pragma once

namespace CoreEngine
{
	class C_GameObject;
}

namespace CoreEngine
{
	class C_Transform
	{
	private:
		D3DXVECTOR3* m_pPosition;
		D3DXVECTOR3* m_pRotation;
		D3DXVECTOR3* m_pScale;

	public:
		PROPERTY(D3DXVECTOR3, position);
		GET(position) { return *m_pPosition; }
		SET(position) { *m_pPosition = _value; }

		PROPERTY(D3DXVECTOR3, rotation);
		GET(rotation) { return *m_pRotation; }
		SET(rotation) { *m_pRotation = _value; }

		PROPERTY(D3DXVECTOR3, scale);
		GET(scale) { return *m_pScale; }
		SET(scale) { *m_pScale = _value; }

	private:
		std::unique_ptr<C_GameObject> m_pGameObject;

	public:
		READONLY_PROPERTY(std::unique_ptr<C_GameObject>&, gameObject);
		const GET(gameObject) { return m_pGameObject; }

	public:
		C_Transform();
		~C_Transform();

	public:
		virtual HRESULT Create();
		virtual void	Destroy();

	};

}