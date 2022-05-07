#include "MarineManager.h"

#include "..\..\CoreEngine\Scripts\Texture.h"

namespace Game
{
	using namespace CoreEngine;

	C_MarineManager::C_MarineManager()
	{
		Create();
	}
	C_MarineManager::~C_MarineManager()
	{
		Destroy();
	}

	HRESULT C_MarineManager::Create()
	{
		m_pUnitTexture = std::make_shared<C_Texture>();

		if (FAILED(m_pUnitTexture->Init(TEXT("Marine.png"))))
			return E_FAIL;

		if (FAILED(m_pUnitTexture->Create()))
		{
			m_pUnitTexture = nullptr;

			MessageBox(NULL, TEXT("마린 텍스쳐 불러오기 실패"), TEXT("오류"), MB_OK);

			return E_FAIL;
		}

		return S_OK;
	}
	void C_MarineManager::Destroy()
	{
		m_pUnitTexture.reset();
	}
}