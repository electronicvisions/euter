#pragma once
#include <boost/serialization/vector.hpp>

#include <vector>
#include <utility>

#ifdef PYPLUSPLUS
#include <stdint.h>
#else
#include <cstdint>
#endif


namespace euter {

/* Spikes */
typedef double spiketime_t; // (experiment-)global spike time (bio, in seconds)
typedef uint32_t neuronindex_t; // global neuron index (bio) TODO: == ID of neurons
typedef std::vector<spiketime_t> spiketimes_t;

class SpikeTrain {
	// simple implementation of NeoBlock
public:
	SpikeTrain();
	SpikeTrain(
		spiketimes_t times
	);
	SpikeTrain(
		spiketimes_t times,
		spiketime_t mTstop,
		spiketime_t mTstart = 0.0
	);

private:
	spiketime_t mTstart; // start of spiketrain/recording
	spiketime_t mTstop; // end of spiketrain/recording
	std::vector<spiketime_t> mTimes;

	friend class boost::serialization::access;
	template<class Archiver>
	void serialize(Archiver & ar, const unsigned int /*version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(mTstart);
		ar & BOOST_SERIALIZATION_NVP(mTstop);
		ar & BOOST_SERIALIZATION_NVP(mTimes);
	}
};

// spikelist: index, []
typedef std::vector<std::pair<neuronindex_t, SpikeTrain> > spikelist_t;

} // namespace euter
