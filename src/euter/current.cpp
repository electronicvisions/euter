#include "euter/current.h"
#include "euter/assembly.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "euter/objectstore.h"

void CurrentSource::inject_into(const boost::shared_ptr<Assembly>& target)
{
	if(mTarget == 0)
	{
		mTarget = target;
	}
	else
	{
		mTarget->append(*target);
	}
}

CurrentSourcePtr DCSource::create(ObjectStore& store, double amplitude, double start, double stop)
{
	CurrentSourcePtr source = boost::make_shared<DCSource>(amplitude, start, stop);

	store.insert(source);
	return source;
}

DCSource::DCSource(double amplitude, double start, double stop) :
	mAmplitude(amplitude),
	mStart(start),
	mStop(stop)
{
}

bool DCSource::operator==(const CurrentSource& right) const
{
	const DCSource* right_ptr = dynamic_cast<const DCSource*>(&right);
	if(!right_ptr) return false;
	return mAmplitude == right_ptr->mAmplitude
	    && mStart == right_ptr->mStart
		&& mStop == right_ptr->mStop;
}

BOOST_CLASS_EXPORT_IMPLEMENT(DCSource)

CurrentSourcePtr StepCurrentSource::create(ObjectStore& store, ublas::vector<double> times, ublas::vector<double> amplitudes)
{
	CurrentSourcePtr source = boost::make_shared<StepCurrentSource>(times, amplitudes);

	store.insert(source);
	return source;
}

StepCurrentSource::StepCurrentSource(ublas::vector<double> times, ublas::vector<double> amplitudes) :
	mTimes(times), mAmplitudes(amplitudes)
{
}

bool StepCurrentSource::operator==(const CurrentSource& right) const
{
	const StepCurrentSource* right_ptr = dynamic_cast<const StepCurrentSource*>(&right);
	if(!right_ptr) return false;
	return mTimes.size() == right_ptr->mTimes.size()
		&& ublas::norm_2(mTimes - right_ptr->mTimes) == 0
		&& mAmplitudes.size() == right_ptr->mAmplitudes.size()
	    && ublas::norm_2(mAmplitudes - right_ptr->mAmplitudes) == 0;
}

BOOST_CLASS_EXPORT_IMPLEMENT(StepCurrentSource)

CurrentSourcePtr ACSource::create(ObjectStore& store, 
				   double amplitude,
				   double offset,
				   double frequency,
				   double phase,
				   double start,
				   double stop)
{
	CurrentSourcePtr source = boost::make_shared<ACSource>(
			amplitude,
			offset,
			frequency,
			phase,
			start,
			stop);
	
	store.insert(source);
	return source;
}

ACSource::ACSource(double amplitude,
				   double offset,
				   double frequency,
				   double phase,
				   double start,
				   double stop) :
	mAmplitude(amplitude),
	mOffset(offset),
	mFrequency(frequency),
	mPhase(phase),
	mStart(start),
	mStop(stop)
{
}

bool ACSource::operator==(const CurrentSource& right) const
{
	const ACSource* right_ptr = dynamic_cast<const ACSource*>(&right);
	if(!right_ptr) return false;
	return mAmplitude == right_ptr->mAmplitude
	    && mOffset == right_ptr->mOffset
	    && mFrequency == right_ptr->mFrequency
	    && mPhase == right_ptr->mPhase
	    && mStart == right_ptr->mStart
		&& mStop == right_ptr->mStop;
}

BOOST_CLASS_EXPORT_IMPLEMENT(ACSource)

CurrentSourcePtr NoisyCurrentSource::create(ObjectStore& store,
									   double mean,
									   double stdev,
									   double dt,
									   double start,
									   double stop)
{
	CurrentSourcePtr source = boost::make_shared<NoisyCurrentSource>(
			mean,
			stdev,
			dt,
			start,
			stop);
	
	store.insert(source);
	return source;
}

NoisyCurrentSource::NoisyCurrentSource(double mean,
									   double stdev,
									   double dt,
									   double start,
									   double stop) :
	mMean(mean), mStdev(stdev), mDt(dt), mStart(start), mStop(stop)
{
}

bool NoisyCurrentSource::operator==(const CurrentSource& right) const
{
	const NoisyCurrentSource* right_ptr = dynamic_cast<const NoisyCurrentSource*>(&right);
	if(!right_ptr) return false;
	return mMean == right_ptr->mMean
	    && mStdev == right_ptr->mStdev
		&& mDt == right_ptr->mDt
		&& mStart == right_ptr->mStart
		&& mStop == right_ptr->mStop;
}

BOOST_CLASS_EXPORT_IMPLEMENT(NoisyCurrentSource)
