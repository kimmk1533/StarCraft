#include "stdafx_Core.h"
#include "TextureManager.h"

namespace CoreEngine
{
	HRESULT C_TextureManager::AddTexture(const std::string& _path, const std::string& _key)
	{
		std::shared_ptr<C_Texture> texture = std::make_shared<C_Texture>();
		if (FAILED(texture->Init(_path)))
			return E_FAIL;
		SAFE_CREATE(texture);

		(*m_pTextureDictionary)[_key] = { 0.0f, texture };

		return S_OK;
	}
	std::shared_ptr<C_Texture> C_TextureManager::GetTexture(const std::string& _key)
	{
		std::unordered_map<std::string, std::pair<FLOAT, std::shared_ptr<C_Texture>>>::iterator iter = m_pTextureDictionary->find(_key);

		if (iter != m_pTextureDictionary->end())
		{
			std::pair<FLOAT, std::shared_ptr<C_Texture>>& pair = iter->second;

			pair.first = 0.0f;

			return pair.second;
		}

		return nullptr;
	}

	C_TextureManager::C_TextureManager()
	{
		m_pTextureDictionary = nullptr;
	}
	C_TextureManager::~C_TextureManager()
	{
		Destroy();
	}

	HRESULT C_TextureManager::Create()
	{
		m_pTextureDictionary = new std::unordered_map<std::string, std::pair<FLOAT, std::shared_ptr<C_Texture>>>();

		return S_OK;
	}
	void C_TextureManager::Destroy()
	{
		SAFE_DELETE(m_pTextureDictionary);
	}

	HRESULT C_TextureManager::Update(const FLOAT& _deltaTime)
	{
		for (auto& item : (*m_pTextureDictionary))
		{
			item.second.first += _deltaTime;
		}

		return S_OK;
	}

}