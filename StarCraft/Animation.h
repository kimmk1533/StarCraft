#pragma once

class CLcTexture;

// 키 프레임 방식으로 애니메이션 재생
class CAnimation : CFrameWork
{
public:
	struct KeyFrame
	{
		std::shared_ptr<CLcTexture*> texture;
		RECT size;
		float time;

		KeyFrame(std::shared_ptr<CLcTexture*> _texture, const RECT _size, float _time)
		{
			texture = _texture;
			size = _size;
			time = _time;
		}
	};

private:
	int m_iFrame;				// 현재 키 프레임
	int m_iFrameCount;			// 키 프레임 갯수
	float m_fTimeToNext;		// 1프레임 당 시간
	float m_fTotalElapsed;		// 경과 시간(타이머)

	bool m_bIsPlaying;			// 재생 여부

	std::vector<KeyFrame*>* m_pKeyFrames; // 재생할 프레임들

public:
	CAnimation();
	virtual ~CAnimation();

	HRESULT	Create() override;
	HRESULT	Update(const float _deltaTime) override;
	//HRESULT Render(CLcSprite* _drawer) override;
	HRESULT Render(CLcSprite* _drawer, D3DXVECTOR2 _pos);
	void	Destroy() override;

private:
	KeyFrame* GetKeyFrame() const;

public:
	void	AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT _size = RECT{ 0, 0, 100, 100 }, float _time = 1.0f);
	void	Play();
	void	Pause();
};
