#pragma once

namespace CoreEngine
{
    class C_Timer;

    class C_Animator : public C_FrameWork, public IUpdatable//, public IRenderable
    {
    public:
        C_Animator();
        virtual ~C_Animator();

    public:
        virtual HRESULT Create() override;
        virtual void	Destroy() override;

        virtual HRESULT Update(const FLOAT& _deltaTime) override;
        //virtual HRESULT Render() override;

    protected:
        C_Timer* m_pAnimTimer;

        DWORD m_dwFuncIndex;
        DWORD m_dwFuncCount;
        std::unordered_multimap<DWORD, std::function<void()>>* m_pFuncList;

    public:
        void SetSampleRate(const FLOAT& _sampleRate);

        /// <summary>
        /// 애니메이션 함수 추가
        /// </summary>
        /// <param name="_index">인덱스(0 == 매 프래임마다)</param>
        /// <param name="_func">함수</param>
        void AddFunc(DWORD _index, std::function<void()> _func);
    };
}