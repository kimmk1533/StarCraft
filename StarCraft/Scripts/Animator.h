#pragma once

namespace Game
{
	using namespace CoreEngine;

	enum class E_Direction : unsigned char;

	enum class E_AnimMode : unsigned char
	{
		Once,						// 한 번만 재생
		Repeat,						// 처음부터 반복 재생
		Repeat_Back,				// 돌아가는 반복 재생

		Max
	};

	enum class E_AnimState : unsigned char
	{
		Init,						// 첫 생성(유닛 태어남, 건물 1단계) 애니메이션
		Death,						// 유닛 사망, 건물 파괴 애니메이션

		Idle,						// 아이들 애니메이션
		Other1,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 애니메이션1
		Other2,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 애니메이션2

		GroundAttackInit,			// 지상 공격 시작 애니메이션
		AirAttackInit,				// 공중 공격 시작 애니메이션

		GroundAttackRepeat,			// 지상 공격 반복 애니메이션
		AirAttackRepeat,			// 공중 공격 반복 애니메이션

		GroundAttackToIdle,			// 지상 공격 종료 애니메이션
		AirAttackToIdle,			// 공중 공격 종료 애니메이션

		CastSpell,					// 마법 시전 애니메이션

		Walking,					// 이동 애니메이션

		// 일꾼 애니메이션
		WorkingToIdle,				// 일꾼 작업 종료 애니메이션

		// 건물 애니메이션
		Built,						// 건물 건설 2단계 애니메이션
		AlmostBuilt,				// 건물 건설 3단계 애니메이션
		IsWorking,					// 생산 or 업그레이드 애니메이션
		Landing,					// 테란 건물 랜딩 시작 애니메이션
		LiftOff,					// 테란 건물 랜딩 종료 애니메이션

		// 저그 버로우 애니메이션
		BurrowInit,					// 버로우 할 때의 애니메이션
		Burrow,						// 버로우 애니메이션
		UnBurrow,					// 언버로우 애니메이션

		Max
	};

	class C_Animator : public C_FrameWork, public IUpdatable, public IRenderable
	{
	private:
		// 키 프레임 방식 애니메이션
		class C_Animation : public C_FrameWork, public IUpdatable, public IRenderable
		{
		public:
			struct S_KeyFrame
			{
				std::shared_ptr<C_Texture> texture;
				RECT size;
				float time;

				S_KeyFrame(std::shared_ptr<C_Texture> _texture, const RECT _size, float _time)
				{
					texture = _texture;
					size = _size;
					time = _time;
				}
			};

		private:
			C_Animator* m_Animator;					// 자신을 재생할 애니메이터
			E_AnimMode m_AnimMode;					// 애니메이션 재생 모드

			int m_iFrame;							// 현재 키 프레임
			int m_iFrameCount;						// 키 프레임 갯수

			float m_fTimeToNext;					// 1프레임 당 시간
			float m_fTotalElapsed;					// 경과 시간(타이머)

			bool m_bIsPlaying;						// 재생 여부
			bool m_bIsRepeat;						// 반복 모드용 변수

			std::vector<S_KeyFrame*>* m_pKeyFrames;	// 재생할 프레임들
			std::unordered_map<int, std::vector<std::function<void()>>>* m_pFuncs;

		public:
			C_Animation(C_Animator* animator);
			virtual ~C_Animation();

			HRESULT	Create() override;
			HRESULT	Update(const float _deltaTime) override;
			HRESULT Render() override;
			void	Destroy() override;

		private:
			S_KeyFrame* GetKeyFrame() const;

		public:
			void	AddFrame(std::shared_ptr<C_Texture>& _texture, const RECT& _size = RECT{ 0, 0, 100, 100 }, float _time = 1.0f);
			HRESULT	AddFunc(int _frameCount, std::function<void()> _func);
			void	Play();
			void	Pause();

			int GetFrameCount();
			E_AnimMode	GetAnimMode();
			void SetAnimMode(const E_AnimMode& _animMode);
		};

	private:
		E_AnimState m_AnimState;
		E_Direction m_Direction;
		E_Direction m_TargetDir;

		D3DXVECTOR2 m_Position;
		D3DXVECTOR3 m_Rotation; // x, y는 회전 중심점. z는 회전 각도(degree)
		D3DXVECTOR2 m_Scale;
		D3DXVECTOR3 m_Offset;
		D3DXCOLOR	m_Color;

	public:
		C_Animator();
		virtual ~C_Animator();

		HRESULT	Create() override;
		HRESULT	Update(const float _deltaTime) override;
		HRESULT Render() override;
		void	Destroy() override;

	protected:
		std::unordered_map<std::pair<E_AnimState, E_Direction>, C_Animation*, Pair_Hash>* m_AnimDictionary;

	public:
		D3DXVECTOR2 GetPosition();
		void SetPosition(float _x, float _y);
		void SetPosition(D3DXVECTOR2 _pos);
		void AddPosition(float _x, float _y);
		void AddPosition(D3DXVECTOR2 _pos);

		D3DXVECTOR3 GetRotation();
		void SetRotation(float _x, float _y, float _degree);
		void SetRotation(D3DXVECTOR3 _rot);

		D3DXVECTOR2 GetScale();
		void SetScale(float _x, float _y);
		void SetScale(D3DXVECTOR2 _scale);

		D3DXVECTOR3 GetOffset();
		void SetOffset(float _x, float _y, float _z = 0.0f);
		void SetOffset(D3DXVECTOR3 _offset);

		D3DXCOLOR GetColor();
		void SetColor(float _r, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f);
		void SetColor(D3DXCOLOR _color);

		HRESULT AddFrame(const E_AnimState& _state, const E_Direction& _direction, std::shared_ptr<C_Texture>& _texture, const RECT _size, float _time);
		HRESULT AddFrame(const std::pair<E_AnimState, E_Direction>& _condition, std::shared_ptr<C_Texture>& _texture, const RECT _size, float _time);
		bool HasAnimState(const E_AnimState& _state, const E_Direction& _direction);
		bool HasAnimState(const std::pair<E_AnimState, E_Direction>& _condition);

		INT GetFrameCount(const E_AnimState& _state, const E_Direction& _direction);

		E_AnimState GetAnimState();
		void SetAnimState(const E_AnimState& _state);
		void SetAnimState(const E_AnimState& _state, const E_Direction& _direction);
		void SetAnimState(const std::pair<E_AnimState, E_Direction>& _condition);

		E_Direction GetDirection();
		void SetDirection(const E_Direction& _direction);

		// 모든 방향에 대하여 설정
		void SetAnimMode(const E_AnimState& _state, const E_AnimMode& _animMode);
		// 특정 방향에 대하여 설정
		void SetAnimMode(const E_AnimState& _state, const E_Direction& _direction, const E_AnimMode& _animMode);
		void SetAnimMode(const std::pair<E_AnimState, E_Direction>& _condition, const E_AnimMode& _animMode);

		HRESULT AddFunc(const E_AnimState& _state, int _frameCount, std::function<void()> _func);
		HRESULT AddFunc(const E_AnimState& _state, const E_Direction& _direction, std::function<void()> _func);
		HRESULT AddFunc(const std::pair<E_AnimState, E_Direction>& _condition, std::function<void()> _func);
		HRESULT AddFunc(const E_AnimState& _state, const E_Direction& _direction, int _frameCount, std::function<void()> _func);
		HRESULT AddFunc(const std::pair<E_AnimState, E_Direction>& _condition, int _frameCount, std::function<void()> _func);
	};
}