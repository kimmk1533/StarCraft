#include "stdafx.h"

#include "Animation.h"

CAnimation::CAnimation()
{
	m_iFrame = 0;
	m_iFrameCount = 0;
	m_fTimeToNext = 0;
	m_fTotalElapsed = 0.0f;
	m_bIsPlaying = true;
	m_pKeyFrames = nullptr;

}

CAnimation::~CAnimation()
{
	Destroy();
}

HRESULT CAnimation::Create()
{
	m_pKeyFrames = new std::vector<KeyFrame*>();

	return S_OK;
}
HRESULT CAnimation::Update(const float _deltaTime)
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
HRESULT CAnimation::Render(CLcSprite* _drawer)
{
	if (m_iFrameCount == 0)
		return E_FAIL;

	KeyFrame* keyframe = GetKeyFrame();
	if (!keyframe)
		return E_FAIL;

	/*if (FAILED(GDEVICE->BeginScene()))
		return E_FAIL;*/

	LPDIRECT3DTEXTURE9 texture = (*keyframe->texture)->GetTexture();

	_drawer->Draw(texture, &(keyframe->size), &m_Scale, &m_Rotation, &m_Position, m_Color);

	//GDEVICE->EndScene();

	return S_OK;
}
void CAnimation::Destroy()
{
	for (int i = 0; i < m_pKeyFrames->size(); ++i)
		SAFE_DELETE((*m_pKeyFrames)[i]);

	SAFE_DELETE(m_pKeyFrames);
}

CAnimation::KeyFrame* CAnimation::GetKeyFrame() const
{
	if (m_iFrameCount == 0)
		return nullptr;

	if (m_iFrame < 0 || m_iFrame >= m_iFrameCount)
		return nullptr;

	return (*m_pKeyFrames)[m_iFrame];
}

void CAnimation::AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time)
{
	std::shared_ptr<CLcTexture*> texture = _texture;
	m_pKeyFrames->push_back(new KeyFrame(_texture, _size, _time));
	if (m_iFrameCount++ == 0)
		m_fTimeToNext = _time;
}

void CAnimation::Play()
{
	m_bIsPlaying = true;
}
void CAnimation::Pause()
{
	m_bIsPlaying = false;
}