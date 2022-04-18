#include "stdafx.h"

#include "Animator.h"

CAnimator::CAnimation::CAnimation(CAnimator* animator)
{
	m_Animator = animator;

	m_iFrame = 0;
	m_iFrameCount = 0;

	m_fTimeToNext = 0;
	m_fTotalElapsed = 0.0f;

	m_bIsPlaying = true;
	m_pKeyFrames = nullptr;
}
CAnimator::CAnimator::CAnimation::~CAnimation()
{
	Destroy();
}

HRESULT CAnimator::CAnimation::Create()
{
	m_pKeyFrames = new std::vector<KeyFrame*>();

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

		if (++m_iFrame >= m_iFrameCount)
			m_iFrame = 0;

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
		return E_FAIL;

	KeyFrame* keyframe = GetKeyFrame();
	if (!keyframe)
		return E_FAIL;

	LPDIRECT3DTEXTURE9 texture = (*keyframe->texture)->GetTexture();

	m_pLcSprite->Draw(texture, &(keyframe->size), &(m_Animator->m_Scale), &(m_Animator->m_Rotation), &(m_Animator->m_Position), m_Animator->m_Color);

	return S_OK;
}
void CAnimator::CAnimation::Destroy()
{
	for (auto item : (*m_pKeyFrames))
		SAFE_DELETE(item);

	SAFE_DELETE(m_pKeyFrames);
}

CAnimator::CAnimation::KeyFrame* CAnimator::CAnimation::GetKeyFrame() const
{
	if (m_iFrameCount == 0)
		return nullptr;

	if (m_iFrame < 0 || m_iFrame >= m_iFrameCount)
		return nullptr;

	return (*m_pKeyFrames)[m_iFrame];
}
void CAnimator::CAnimation::AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time)
{
	std::shared_ptr<CLcTexture*> texture = _texture;
	m_pKeyFrames->push_back(new KeyFrame(_texture, _size, _time));
	if (m_iFrameCount++ == 0)
		m_fTimeToNext = _time;
}
void CAnimator::CAnimation::Play()
{
	m_bIsPlaying = true;
}
void CAnimator::CAnimation::Pause()
{
	m_bIsPlaying = false;
}

/// <summary>
/// 
/// </summary>

CAnimator::CAnimator()
{
	m_AnimState = E_AnimState::Walking;

	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_AnimDictionary = nullptr;
}

CAnimator::~CAnimator()
{
	Destroy();
}

HRESULT CAnimator::Create()
{
	m_AnimDictionary = new std::unordered_map<E_AnimState, CAnimation*>();

	return S_OK;
}

HRESULT CAnimator::Update(const float _deltaTime)
{
	CAnimation* animation = nullptr;
	try
	{
		animation = m_AnimDictionary->at(m_AnimState);
	}
	catch (const std::exception& e)
	{
		if (nullptr == animation)
		{
			MessageBox(NULL, L"Error: CAnimator::Update", L"Error!", MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(animation->Update(_deltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimator::Render()
{
	CAnimation* animation = nullptr;
	try
	{
		animation = m_AnimDictionary->at(m_AnimState);
	}
	catch (const std::exception& e)
	{
		if (nullptr == animation)
		{
			MessageBox(NULL, L"Error: CAnimator::Render", L"Error!", MB_OK);
			return E_FAIL;
		}
	}

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

void CAnimator::Position(float _x, float _y)
{
	this->Position(D3DXVECTOR2(_x, _y));
}
void CAnimator::Position(D3DXVECTOR2 _pos)
{
	m_Position = _pos;
}

void CAnimator::Rotation(float _x, float _y, float _degree)
{
	this->Rotation(D3DXVECTOR3(_x, _y, _degree));
}
void CAnimator::Rotation(D3DXVECTOR3 _rot)
{
	m_Rotation = _rot;
}

void CAnimator::Scale(float _x, float _y)
{
	this->Scale(D3DXVECTOR2(_x, _y));
}
void CAnimator::Scale(D3DXVECTOR2 _scale)
{
	m_Scale = _scale;
}

void CAnimator::Color(float _r, float _g, float _b, float _a)
{
	this->Color(D3DXCOLOR(_r, _g, _b, _a));
}
void CAnimator::Color(D3DXCOLOR _color)
{
	m_Color = _color;
}

HRESULT CAnimator::AddFrame(E_AnimState state, std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time)
{
	CAnimation* animation = nullptr;
	try
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
	}

	animation->AddFrame(_texture, _size, _time);

	return S_OK;
}
