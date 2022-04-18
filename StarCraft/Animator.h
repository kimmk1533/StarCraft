#pragma once
//#include "Animation.h"

enum class E_AnimState
{
	Init,
	Death,

	Idle,
	Other,

	GroundAttackInit,
	AirAttackInit,
	GroundAttackRepeat,
	AirAttackRepeat,
	CastSpell,
	GroundAttackToIdle,
	AirAttackToIdle,

	Walking,
	WalkingToIdle,

	Built,
	AlmostBuilt,
	Landing,
	LiftOff,

	IsWorking,
	WorkingToIdle,

	Burrow,
	UnBurrow,

	Max
};

class CAnimator : public CFrameWork
{
private:
	E_AnimState m_AnimState;

	D3DXVECTOR2 m_Position;
	D3DXVECTOR3 m_Rotation; // x, y는 회전 중심점. z는 회전 각도(degree)
	D3DXVECTOR2 m_Scale;
	D3DXCOLOR	m_Color;

	// 키 프레임 방식 애니메이션
	class CAnimation : public CFrameWork
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
		CAnimator* m_Animator;		// 애니메이터

		int m_iFrame;				// 현재 키 프레임
		int m_iFrameCount;			// 키 프레임 갯수

		float m_fTimeToNext;		// 1프레임 당 시간
		float m_fTotalElapsed;		// 경과 시간(타이머)

		bool m_bIsPlaying;			// 재생 여부

		std::vector<KeyFrame*>* m_pKeyFrames; // 재생할 프레임들

	public:
		CAnimation(CAnimator* animator);
		virtual ~CAnimation();

		HRESULT	Create() override;
		HRESULT	Update(const float _deltaTime) override;
		HRESULT Render() override;
		void	Destroy() override;

	private:
		KeyFrame* GetKeyFrame() const;

	public:
		void	AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT _size = RECT{ 0, 0, 100, 100 }, float _time = 1.0f);
		void	Play();
		void	Pause();
	};
public:
	CAnimator();
	virtual ~CAnimator();

	HRESULT	Create() override;
	HRESULT	Update(const float _deltaTime) override;
	HRESULT Render() override;
	void	Destroy() override;

protected:
	std::unordered_map<E_AnimState, CAnimation*>* m_AnimDictionary;

public:
	D3DXVECTOR2 Position() { return m_Position; }
	void Position(float _x, float _y);
	void Position(D3DXVECTOR2 _pos);

	D3DXVECTOR3 Rotation() { return m_Rotation; }
	void Rotation(float _x, float _y, float _degree);
	void Rotation(D3DXVECTOR3 _rot);

	D3DXVECTOR2 Scale() { return m_Scale; }
	void Scale(float _x, float _y);
	void Scale(D3DXVECTOR2 _scale);

	D3DXCOLOR Color() { return m_Color; }
	void Color(float _r, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f);
	void Color(D3DXCOLOR _color);

	HRESULT AddFrame(E_AnimState state, std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time);
};