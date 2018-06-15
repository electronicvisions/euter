#include "helper_spikes.h"

#include <algorithm>


SpikeTrain::SpikeTrain()
:
	mTstart(),
	mTstop(),
	mTimes()
{}

SpikeTrain::SpikeTrain(
	spiketimes_t times
) :
	mTimes(times)
{
	std::sort(mTimes.begin(), mTimes.end());
	mTstart = times.front();
	mTstop = times.back();
}


SpikeTrain::SpikeTrain(
	spiketimes_t times,
	spiketime_t t_stop,
	spiketime_t t_start
) :
	mTstart(t_start),
	mTstop(t_stop),
	mTimes(times)
{}
