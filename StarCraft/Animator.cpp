#include "stdafx.h"

#include "Animator.h"

CAnimator::CAnimation::CAnimation(CAnimator* animator)
{
	m_Animator = animator;
	m_AnimMode = E_AnimMode::Repeat;

	m_iFrame = 0;
	m_iFrameCount = 0;

	m_fTimeToNext = 0;
	m_fTotalElapsed = 0.0f;

	m_bIsPlaying = true;
	m_bIsRepeat = false;

	m_pKeyFrames = nullptr;
	m_pFuncs = nullptr;
}
CAnimator::CAnimator::CAnimation::~CAnimation()
{
	Destroy();
}

HRESULT CAnimator::CAnimation::Create()
{
	m_pKeyFrames = new std::vector<KeyFrame*>();
	m_pFuncs = new std::unordered_map<int, std::vector<std::function<void()>>>();

	return S_OK;
}
HRESULT CAnimator::CAnimation::Update(const float _deltaTime)
{
	if (m_pKeyFrames->size() == 0)
		return E_FAIL;

	if (!m_bIsPlaying)
		return S_OK;

	m_fTotalElapsed += _deltaTime;
	if (m_fTotalElapsed > m_fTimeToNext)
	{
		m_fTotalElapsed -= m_fTimeToNext;

		switch (m_AnimMode)
		{
		case E_AnimMode::Once:
			if (++m_iFrame >= m_iFrameCount - 1)
				m_bIsPlaying = false;
			break;
		case E_AnimMode::Repeat:
			if (++m_iFrame >= m_iFrameCount)
				m_iFrame = 0;
			break;
		case E_AnimMode::Repeat_Back:
			if (!m_bIsRepeat)
			{
				if (++m_iFrame >= m_iFrameCount - 1)
					m_bIsRepeat = true;
			}
			else
			{
				if (--m_iFrame <= 0)
					m_bIsRepeat = false;
			}
			break;
		default:
			MessageBox(nullptr, L"Error: Animation Mode", L"Error!", MB_OK);
			return E_FAIL;
		}

		for (auto item : (*m_pFuncs)[m_iFrame])
		{
			item();
		}

		KeyFrame* keyframe = GetKeyFrame();
		if (!keyframe)
			return E_FAIL;

		m_fTimeToNext = keyframe->time;
	}

	return S_OK;
}
HRESULT CAnimator::CAnimation::Render()
{
	if (m_iFrameCount == 0)
	{
		MessageBox(nullptr, L"Error: CAnimation::m_iFrameCount == 0", L"Error!", MB_OK);
		return E_FAIL;
	}

	KeyFrame* keyframe = GetKeyFrame();
	if (!keyframe)
	{
		MessageBox(nullptr, L"Error: CAnimation::keyframe is nullptr", L"Error!", MB_OK);
		return E_FAIL;
	}

	LPDIRECT3DTEXTURE9 texture = (*keyframe->texture)->GetTexture();

	/*D3DXVECTOR2 pos = m_Animator->m_Position;
	D3DXVECTOR3 rot = m_Animator->m_Rotation;
	D3DXVECTOR2 scale = m_Animator->m_Scale;*/

	E_Direction direction = m_Animator->GetDirection();
	if (direction > E_Direction::Down)
		m_Animator->SetScale(-1.0f, 1.0f);
	else
		m_Animator->SetScale(1.0f, 1.0f);

	m_pLcSprite->Draw(texture, &(keyframe->size), &(m_Animator->m_Scale), &(m_Animator->m_Rotation), &(m_Animator->m_Position), &(m_Animator->m_Offset), m_Animator->m_Color);

	return S_OK;
}
void CAnimator::CAnimation::Destroy()
{
	for (auto item : (*m_pKeyFrames))
		SAFE_DELETE(item);

	SAFE_DELETE(m_pKeyFrames);
	SAFE_DELETE(m_pFuncs);
}

CAnimator::CAnimation::KeyFrame* CAnimator::CAnimation::GetKeyFrame() const
{
	if (m_iFrameCount == 0)
		return nullptr;

	if (m_iFrame < 0 || m_iFrame >= m_iFrameCount)
		return nullptr;

	return (*m_pKeyFrames)[m_iFrame];
}
void CAnimator::CAnimation::AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT& _size, float _time)
{
	std::shared_ptr<CLcTexture*> texture = _texture;
	m_pKeyFrames->push_back(new KeyFrame(_texture, _size, _time));
	if (m_iFrameCount++ == 0)
		m_fTimeToNext = _time;
}
HRESULT CAnimator::CAnimation::AddFunc(int _frameCount, std::function<void()> _func)
{
	if (_frameCount < 0 || _frameCount >= m_iFrameCount)
		return E_FAIL;

	(*m_pFuncs)[_frameCount].push_back(_func);

	return S_OK;
}
void CAnimator::CAnimation::Play()
{
	m_bIsPlaying = true;
}
void CAnimator::CAnimation::Pause()
{
	m_bIsPlaying = false;
}

int CAnimator::CAnimation::GetFrameCount()
{
	return m_iFrameCount;
}
E_AnimMode CAnimator::CAnimation::GetAnimMode()
{
	return m_AnimMode;
}
void CAnimator::CAnimation::SetAnimMode(const E_AnimMode& _animMode)
{
	m_AnimMode = _animMode;
}

/// <summary>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>

CAnimator::CAnimator()
{
	m_AnimState = E_AnimState::Init;
	m_Direction = E_Direction::Right_Down_Down;

	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	m_Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_AnimDictionary = nullptr;
}
CAnimator::~CAnimator()
{
	Destroy();
}

HRESULT CAnimator::Create()
{
	m_AnimDictionary = new std::unordered_map<std::pair<E_AnimState, E_Direction>, CAnimation*, Pair_Hash>();
	for (int i = 0; i < static_cast<int>(E_AnimState::Max); ++i)
	{
		for (int j = 0; j < static_cast<int>(E_Direction::Max); ++j)
		{
			//std::pair<E_AnimState, E_Direction> state = std::make_pair<E_AnimState, E_Direction>(static_cast<E_AnimState>(i), static_cast<E_Direction>(j));
			//(*m_AnimDictionary)[state] = new CAnimation(this);
			(*m_AnimDictionary)[{ static_cast<E_AnimState>(i), static_cast<E_Direction>(j) }] = new CAnimation(this);
			if (FAILED(((*m_AnimDictionary)[{ static_cast<E_AnimState>(i), static_cast<E_Direction>(j) }])->Create()))
				return E_FAIL;
		}
	}

	return S_OK;
}
HRESULT CAnimator::Update(const float _deltaTime)
{
	CAnimation* animation = m_AnimDictionary->at({ m_AnimState, m_Direction });
	/*CAnimation* animation = nullptr;
	try
	{
		animation = m_AnimDictionary->at(std::pair<E_AnimState, E_Direction>(m_AnimState, m_Direction));
	}
	catch (const std::exception&)
	{
		if (nullptr == animation)
		{
			MessageBox(NULL, L"Error: CAnimator::Update", L"Error!", MB_OK);
			return E_FAIL;
		}
	}*/

	if (FAILED(animation->Update(_deltaTime)))
		return E_FAIL;

	return S_OK;
}
HRESULT CAnimator::Render()
{
	CAnimation* animation = m_AnimDictionary->at({ m_AnimState, m_Direction });
	/*CAnimation* animation = nullptr;
	try
	{
		animation = m_AnimDictionary->at(std::pair<E_AnimState, E_Direction>(m_AnimState, m_Direction));
	}
	catch (const std::exception&)
	{
		if (nullptr == animation)
		{
			MessageBox(NULL, L"Error: CAnimator::Render", L"Error!", MB_OK);
			return E_FAIL;
		}
	}*/

	if (FAILED(animation->Render()))
		return E_FAIL;

	return S_OK;
}
void CAnimator::Destroy()
{
	for (auto item : (*m_AnimDictionary))
		SAFE_DELETE(item.second);

	SAFE_DELETE(m_AnimDictionary);
}

// Position
D3DXVECTOR2 CAnimator::GetPosition() { return m_Position; }
void CAnimator::SetPosition(float _x, float _y)
{
	this->SetPosition(D3DXVECTOR2(_x, _y));
}
void CAnimator::SetPosition(D3DXVECTOR2 _pos)
{
	m_Position = _pos;
}
void CAnimator::AddPosition(float _x, float _y)
{
	this->AddPosition(D3DXVECTOR2(_x, _y));
}
void CAnimator::AddPosition(D3DXVECTOR2 _pos)
{
	m_Position += _pos;
}

// Rotation
D3DXVECTOR3 CAnimator::GetRotation() { return m_Rotation; }
void CAnimator::SetRotation(float _x, float _y, float _degree)
{
	this->SetRotation(D3DXVECTOR3(_x, _y, _degree));
}
void CAnimator::SetRotation(D3DXVECTOR3 _rot)
{
	m_Rotation = _rot;
}

// Scale
D3DXVECTOR2 CAnimator::GetScale() { return m_Scale; }
void CAnimator::SetScale(float _x, float _y)
{
	this->SetScale(D3DXVECTOR2(_x, _y));
}
void CAnimator::SetScale(D3DXVECTOR2 _scale)
{
	m_Scale = _scale;
}

// Offset
D3DXVECTOR3 CAnimator::GetOffset() { return m_Offset; }
void CAnimator::SetOffset(float _x, float _y, float _z)
{
	this->SetOffset(D3DXVECTOR3(_x, _y, _z));
}
void CAnimator::SetOffset(D3DXVECTOR3 _offset)
{
	m_Offset = _offset;
}

// Color
D3DXCOLOR CAnimator::GetColor() { return m_Color; }
void CAnimator::SetColor(float _r, float _g, float _b, float _a)
{
	this->SetColor(D3DXCOLOR(_r, _g, _b, _a));
}
void CAnimator::SetColor(D3DXCOLOR _color)
{
	m_Color = _color;
}

// Animation Frame
HRESULT CAnimator::AddFrame(const E_AnimState& _state, const E_Direction& _direction, std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time)
{
	return this->AddFrame({ _state, _direction }, _texture, _size, _time);
}
HRESULT CAnimator::AddFrame(const std::pair<E_AnimState, E_Direction>& _condition, std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time)
{
	CAnimation* animation = m_AnimDictionary->at(_condition);
	/*try
	{
		animation = m_AnimDictionary->at(state);
	}
	catch (const std::exception& e)
	{
		animation = (*m_AnimDictionary)[state] = new CAnimation(this);
		if (FAILED(animation->Create()))
			return E_FAIL;
	}

	if (nullptr == animation)
	{
		MessageBox(NULL, L"Error: CAnimator::AddFrame", L"Error!", MB_OK);
		return E_FAIL;
	}*/

	animation->AddFrame(_texture, _size, _time);

	return S_OK;
}

bool CAnimator::HasAnimState(const E_AnimState& _state, const E_Direction& _direction)
{
	return this->HasAnimState({ _state, _direction });
}
bool CAnimator::HasAnimState(const std::pair<E_AnimState, E_Direction>& _condition)
{
	return m_AnimDictionary->count(_condition);
}

INT CAnimator::GetFrameCount(const E_AnimState& _state, const E_Direction& _direction)
{
	return (*m_AnimDictionary)[{ _state, _direction }]->GetFrameCount();
}

E_AnimState CAnimator::GetAnimState() { return m_AnimState; }
void CAnimator::SetAnimState(const E_AnimState& _state)
{
	if (!HasAnimState(_state, m_Direction))
		return;

	m_AnimState = _state;
}
void CAnimator::SetAnimState(const E_AnimState& _state, const E_Direction& _direction)
{
	this->SetAnimState({ _state, _direction });
}
void CAnimator::SetAnimState(const std::pair<E_AnimState, E_Direction>& _condition)
{
	if (!HasAnimState(_condition))
		return;

	m_AnimState = _condition.first;
	m_Direction = _condition.second;
}

E_Direction CAnimator::GetDirection() { return m_Direction; }
void CAnimator::SetDirection(const E_Direction& _direction)
{
	if (!HasAnimState(m_AnimState, _direction))
		return;

	m_Direction = _direction;
}

void CAnimator::SetAnimMode(const E_AnimState& _state, const E_AnimMode& _animMode)
{
	int max = static_cast<int>(E_Direction::Max);
	for (int i = 0; i < max; ++i)
	{
		E_Direction dir = static_cast<E_Direction>(i);
		this->SetAnimMode({ _state, dir }, _animMode);
	}
}
void CAnimator::SetAnimMode(const E_AnimState& _state, const E_Direction& _direction, const E_AnimMode& _animMode)
{
	this->SetAnimMode({ _state, _direction }, _animMode);
}
void CAnimator::SetAnimMode(const std::pair<E_AnimState, E_Direction>& _condition, const E_AnimMode& _animMode)
{
	(*m_AnimDictionary)[_condition]->SetAnimMode(_animMode);
}

HRESULT CAnimator::AddFunc(const E_AnimState& _state, int _frameCount, std::function<void()> _func)
{
	int max = static_cast<int>(E_Direction::Max);
	for (int i = 0; i < max; ++i)
	{
		E_Direction dir = static_cast<E_Direction>(i);
		if (FAILED(this->AddFunc({ _state, dir }, _frameCount, _func)))
			return E_FAIL;
	}

	return S_OK;
}
HRESULT CAnimator::AddFunc(const E_AnimState& _state, const E_Direction& _direction, std::function<void()> _func)
{
	return this->AddFunc({ _state, _direction }, (*m_AnimDictionary)[{_state, _direction}]->GetFrameCount() - 1, _func);
}
HRESULT CAnimator::AddFunc(const std::pair<E_AnimState, E_Direction>& _condition, std::function<void()> _func)
{
	return this->AddFunc(_condition, (*m_AnimDictionary)[_condition]->GetFrameCount() - 1, _func);
}
HRESULT CAnimator::AddFunc(const E_AnimState& _state, const E_Direction& _direction, int _frameCount, std::function<void()> _func)
{
	return this->AddFunc({ _state, _direction }, _frameCount, _func);
}
HRESULT CAnimator::AddFunc(const std::pair<E_AnimState, E_Direction>& _condition, int _frameCount, std::function<void()> _func)
{
	return (*m_AnimDictionary)[_condition]->AddFunc(_frameCount, _func);
}