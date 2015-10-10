#ifndef _INC_RANDGENERATOR_
#define _INC_RANDGENERATOR_
//////////////////////////////////////////////////////////////////////////
class RandGenerator
{
public:
	RandGenerator()
	{
		m_lRandSeed = 1;
	}

public:
	void SetSeed(long _lSeed)
	{
		m_lRandSeed = _lSeed;
	}
	long GetSeed()
	{
		return m_lRandSeed;
	}
	int Rand()
	{
		m_lRandSeed = ((m_lRandSeed * 214013L + 2531011L) >> 16) & 0x7fff;
		return (int)m_lRandSeed;
	}
	int Rand(int _nMin, int _nMax)
	{
		return _nMin + Rand() % (_nMax - _nMin + 1);
	}

private:
	long m_lRandSeed;
};
//////////////////////////////////////////////////////////////////////////
#endif