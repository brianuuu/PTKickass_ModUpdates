#pragma once

class HudResult
{
public:
	enum ResultState : int
	{
		Idle,
		MainWait,
		Main,
		NewRecord,
		Rank,
		Footer,
		FadeOut,
	};

	enum ResultNumType : int
	{
		TIME,
		RINGS,
		SPEED,
		ENEMY,
		TRICKS,
		TOTAL,
		COUNT
	};

	enum ResultRankType : int
	{
		D,
		C,
		B,
		A,
		S,
		E
	};

	struct ResultData
	{
		int m_score;
		ResultRankType m_rank;
		ResultRankType m_perfectRank;
		int m_nextRankTime;
		float m_totalProp;	// result progress bar (time prop + ring prop) 
		float m_timeProp;	// result progress bar (time prop)
	};

	struct ResultSoundState
	{
		bool m_bar[ResultNumType::COUNT];
		bool m_total;
		bool m_rank;
		bool m_rankVoice;

		ResultSoundState()
			: m_bar{false, false, false, false, false, false}
			, m_total(false)
			, m_rank(false)
			, m_rankVoice(false)
		{}
	};

	struct StageData
	{
		float m_maxSpeed;
		int m_enemyCount;
		int m_trickCount;
	};

	static void Install();
};

