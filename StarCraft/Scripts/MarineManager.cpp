#include "stdafx.h"
#include "MarineManager.h"

namespace Game
{
	C_MarineManager::C_MarineManager()
	{

	}
	C_MarineManager::~C_MarineManager()
	{
		Destroy();
	}

	HRESULT C_MarineManager::Create()
	{
		if (FAILED(C_UnitManager::Create()))
			return E_FAIL;

		m_rcUnitSize = RECT{ 0, 0, 64, 64 };

		m_pOffset = std::make_shared<D3DXVECTOR3>(32.0f, 32.0f, 1.0f);

#pragma region Animation
		int max = static_cast<int>(E_Direction::Max);
		E_UnitState state;
		E_Direction direction;
		RECT rect;

		/* 0, 64, 128, 194, 256, 320, 384, 448, 512, 576, 640, 704, 768 */
		for (int i = 0; i < max; ++i)
		{
			direction = static_cast<E_Direction>(i);

			if (i < 9)
			{
				rect.top = i * 64;
				rect.bottom = (i + 1) * 64;
			}
			else
			{
				rect.top = (16 - i) * 64;
				rect.bottom = (16 - i + 1) * 64;
			}

#pragma region Init
			state = E_UnitState::Init;

			rect.left = 256; rect.right = 320;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region Idle
			state = E_UnitState::Idle;

			rect.left = 256; rect.right = 320;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region Other
			state = E_UnitState::Other1;
			rect.left = 0; rect.right = 192;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region GroundAttack
			state = E_UnitState::GroundAttackInit;
			rect.left = 0; rect.right = 128;
			this->SetTextureRect({ state, direction }, rect);

			state = E_UnitState::GroundAttackRepeat;
			rect.left = 128; rect.right = 256;
			this->SetTextureRect({ state, direction }, rect);

			state = E_UnitState::GroundAttackToIdle;
			rect.left = 256; rect.right = 320;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region AirAttack
			state = E_UnitState::AirAttackInit;
			rect.left = 0; rect.right = 128;
			this->SetTextureRect({ state, direction }, rect);

			state = E_UnitState::AirAttackRepeat;
			rect.left = 128; rect.right = 256;
			this->SetTextureRect({ state, direction }, rect);

			state = E_UnitState::AirAttackToIdle;
			rect.left = 256; rect.right = 320;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region Walking
			state = E_UnitState::Walking;
			rect.left = 256; rect.right = 832;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion

#pragma region Death
			state = E_UnitState::Death;
			rect.top = 576; rect.bottom = 640;
			rect.left = 0; rect.right = 512;
			this->SetTextureRect({ state, direction }, rect);
#pragma endregion
		}
#pragma endregion

		return S_OK;
	}
	void C_MarineManager::Destroy()
	{

	}

	HRESULT C_MarineManager::Update(const FLOAT& _deltaTime)
	{
		if (FAILED(C_UnitManager::Update(_deltaTime)))
			return E_FAIL;

		return S_OK;
	}
	HRESULT C_MarineManager::Render()
	{
		if (FAILED(C_UnitManager::Render()))
			return E_FAIL;

		return S_OK;
	}
}