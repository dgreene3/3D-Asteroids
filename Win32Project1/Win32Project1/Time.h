#ifndef TIME_H
#define TIME_H

#include "Precompiled.h"




class Time {

public:
	Time();

	float TotalTime() const; //in seconds
	float DeltaTime() const; //in seconds

	void Reset();
	void Start(); //call when unpaused
	void Stop(); //call when paused
	void Tick(); //call every frame


public:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;

};

#endif




