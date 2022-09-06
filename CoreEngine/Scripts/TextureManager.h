#pragma once
#include "Singleton.h"

namespace CoreEngine
{
	class C_TextureManager : public C_Singleton<C_TextureManager>, public IFrameWork, public IUpdatable
	{
		friend class C_Singleton<C_TextureManager>;

	private:
		std::unordered_map<std::string, std::pair<FLOAT, std::shared_ptr<C_Texture>>>* m_pTextureDictionary;

	public:
		HRESULT AddTexture(const std::string& _path, const std::string& _key);
		std::shared_ptr<C_Texture> GetTexture(const std::string& _key);

	private:
		C_TextureManager();
		virtual ~C_TextureManager();

		C_TextureManager(const C_TextureManager& _other) = delete;
		void operator=(const C_TextureManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime = 0) override;

	};
}