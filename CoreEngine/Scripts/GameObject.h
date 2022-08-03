#pragma once

namespace CoreEngine
{
	class C_Transform;
}

namespace CoreEngine
{
	class C_GameObject : public C_FrameWork
	{
	private:
		std::unique_ptr<C_Transform> m_pTransform;

	public:
		READONLY_PROPERTY(std::unique_ptr<C_Transform>&, transform);
		GET(transform) { return m_pTransform; }

	public:
		C_GameObject();
		virtual ~C_GameObject();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	};

}