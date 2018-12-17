#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include "assembly.h"

namespace euter {

class ObjectStore;
class CurrentSource;

typedef boost::shared_ptr<CurrentSource> CurrentSourcePtr;
typedef boost::shared_ptr<CurrentSource const> ConstCurrentSourcePtr;

class CurrentSource
{
public:
	virtual ~CurrentSource() {}
	//virtual CurrentSourcePtr create(ObjectStore&/* store*/) {}
	void inject_into(const boost::shared_ptr<Assembly>& target);

	boost::shared_ptr<Assembly const> target() const
	{
		return mTarget;
	}

	virtual bool operator==(const CurrentSource&/* right*/) const = 0;

private:
	boost::shared_ptr<Assembly> mTarget;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(mTarget);
	}
};

class DCSource : public CurrentSource
{
public:
	DCSource() {}
	~DCSource() {}
	DCSource(double amplitude, double start, double stop=0);
	static CurrentSourcePtr create(ObjectStore& store, double amplitude, double start, double stop);
	bool operator==(const CurrentSource& right) const;
private:
	double mAmplitude;
	double mStart;
	double mStop;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CurrentSource)
		   & BOOST_SERIALIZATION_NVP(mAmplitude)
		   & BOOST_SERIALIZATION_NVP(mStart)
		   & BOOST_SERIALIZATION_NVP(mStop);
	}
};


class StepCurrentSource : public CurrentSource
{
public:
	StepCurrentSource() {}
	~StepCurrentSource() {}
	StepCurrentSource(ublas::vector<double> times, ublas::vector<double> amplitudes);
	static CurrentSourcePtr create(ObjectStore& store, ublas::vector<double> times, ublas::vector<double> amplitudes);
	bool operator==(const CurrentSource& right) const;

	ublas::vector<double> const& times() const { return mTimes; }
	ublas::vector<double> const& amplitudes() const { return mAmplitudes; }
private:
	ublas::vector<double> mTimes;
	ublas::vector<double> mAmplitudes;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CurrentSource)
		   & BOOST_SERIALIZATION_NVP(mTimes)
		   & BOOST_SERIALIZATION_NVP(mAmplitudes);
	}
};


class ACSource : public CurrentSource
{
public:
	ACSource() {}
	~ACSource() {}
	ACSource(double amplitude,
			 double offset,
			 double frequency,
			 double phase,
			 double start,
			 double stop);
	static CurrentSourcePtr create(ObjectStore& store, 
			 double amplitude,
			 double offset,
			 double frequency,
			 double phase,
			 double start,
			 double stop);
	bool operator==(const CurrentSource& right) const;
private:
	double mAmplitude;
	double mOffset;
	double mFrequency;
	double mPhase;
	double mStart;
	double mStop;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CurrentSource)
		   & BOOST_SERIALIZATION_NVP(mAmplitude)
		   & BOOST_SERIALIZATION_NVP(mOffset)
		   & BOOST_SERIALIZATION_NVP(mFrequency)
		   & BOOST_SERIALIZATION_NVP(mPhase)
		   & BOOST_SERIALIZATION_NVP(mStart)
		   & BOOST_SERIALIZATION_NVP(mStop);
	}
};


class NoisyCurrentSource : public CurrentSource
{
public:
	NoisyCurrentSource() {}
	~NoisyCurrentSource() {}
	NoisyCurrentSource(double mean,
					   double stdev,
					   double dt,
					   double start,
					   double stop);
	static CurrentSourcePtr create(ObjectStore& store, 
					   double mean,
					   double stdev,
					   double dt,
					   double start,
					   double stop);
	bool operator==(const CurrentSource& right) const;
private:
	double mMean;
	double mStdev;
	double mDt;
	double mStart;
	double mStop;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CurrentSource)
		   & BOOST_SERIALIZATION_NVP(mMean)
		   & BOOST_SERIALIZATION_NVP(mStdev)
		   & BOOST_SERIALIZATION_NVP(mDt)
		   & BOOST_SERIALIZATION_NVP(mStart)
		   & BOOST_SERIALIZATION_NVP(mStop);
	}
};

} // namespace euter

BOOST_CLASS_EXPORT_KEY(euter::DCSource)
BOOST_CLASS_EXPORT_KEY(euter::StepCurrentSource)
BOOST_CLASS_EXPORT_KEY(euter::ACSource)
BOOST_CLASS_EXPORT_KEY(euter::NoisyCurrentSource)
