#pragma once

#include <sstream>

#include "random.h"
#include "nativerandomgenerator.h"

namespace euter {

class NativeRandomDistribution : public RandomDistribution
{
public:
	typedef boost::shared_ptr<NativeRandomGenerator> rng_type;
	typedef NativeRandomGenerator::rng_type raw_rng_type;

	NativeRandomDistribution(const rng_type & rng) : mRng(rng) {}

	static const distribution_int_t int_max = std::numeric_limits<distribution_int_t>::max();

protected:
	NativeRandomDistribution() {}

	raw_rng_type & rng()
	{ return mRng->raw(); }

private:
	rng_type mRng;

	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int &)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RandomDistribution)
		   & BOOST_SERIALIZATION_NVP(mRng);
	}
	friend class boost::serialization::access;
};

} // namespace euter

BOOST_SERIALIZATION_ASSUME_ABSTRACT(euter::NativeRandomDistribution)
BOOST_CLASS_EXPORT_KEY(euter::NativeRandomDistribution)

namespace euter {

template <typename Dist>
class NativeRandomDistributionImpl : public NativeRandomDistribution
{
public:
	typedef NativeRandomDistribution base_type;
	typedef Dist dist_type;
	typedef typename dist_type::result_type result_type;
	typedef typename base_type::rng_type rng_type;
	\
	NativeRandomDistributionImpl(const rng_type & rng, Dist dist) :
		NativeRandomDistribution(rng), mDistribution(dist)
	{}

	virtual void next(result_type & v)
	{
		v = mDistribution(rng());
		switch(constrain())
		{
		case RandomDistribution::REDRAW:
			while(!mRange.test(v))
				v = mDistribution(rng());
			break;
		case RandomDistribution::CLIP:
			mRange.clip(v);
			break;
		default:
			;
		}
	}

	virtual void next(std::vector<result_type> & v)
	{ next(v.begin(), v.end()); }

	template<typename Iterator>
	void next(Iterator in, Iterator end)
	{
		switch(constrain())
		{
		case RandomDistribution::REDRAW:
			while(in != end)
			{
				*in = mDistribution(rng());
				while(!mRange.test(*in))
					*in = mDistribution(rng());
				++in;
			}
			break;
		case RandomDistribution::CLIP:
			while(in != end)
			{
				*in = mDistribution(rng());
				mRange.clip(*in);
				++in;
			}
			break;
		default:
			while(in != end)
			{
				*in = mDistribution(rng()); ++in;
			}
		}
	}

	virtual void setBoundaries(result_type a, result_type b)
	{ mRange.set(a,b); }

	virtual RandomDistribution::ResultType type() const
	{ return RandomDistributionTraits<result_type>::resultType; }

protected:
	NativeRandomDistributionImpl() {}

private:
	dist_type mDistribution;
	NumericRange<result_type> mRange;

	template<typename Archiver>
	void save(Archiver& ar, const unsigned int&) const {
		std::stringstream stream;
		stream << mDistribution;
		std::string state = stream.str();
		ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_type)
		   << BOOST_SERIALIZATION_NVP(state);
	}
	template<typename Archiver>
	void load(Archiver& ar, const unsigned int& /*version*/) {
		std::string state;
		ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_type)
		   >> BOOST_SERIALIZATION_NVP(state);
		std::stringstream stream(state);
		stream >> mDistribution;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	friend class boost::serialization::access;
};

} // namespace euter

// LOAD Boost Preprocessor Macrons
#define INIT 1
#include "nativerandomdistributions_pp.h"
#undef INIT

NATIVE_DISTRIBUTION(NativeRandIntDistribution, std::uniform_int_distribution, distribution_int_t, \
		(distribution_int_t)(a)(0), (distribution_int_t)(b)(int_max))

NATIVE_DISTRIBUTION(NativeUniformDistribution, std::uniform_real_distribution, distribution_float_t, \
		(distribution_float_t)(a)(0.0), (distribution_float_t)(b)(1.0))

NATIVE_DISTRIBUTION(NativeNormalDistribution, std::normal_distribution, distribution_float_t, \
		(distribution_float_t)(mu)(0.0), (distribution_float_t)(std)(1.0))

// TODO this evil thing produces bools :(
//NATIVE_DISTRIBUTION(NativeBernoulliDistribution, std::bernoulli_distribution, distribution_int_t,
//		(distribution_float_t)(p)(1.0))

NATIVE_DISTRIBUTION(NativeBinomialDistribution, std::binomial_distribution, distribution_int_t, \
		(distribution_int_t)(t)(1), (distribution_float_t)(p)(0.5))

NATIVE_DISTRIBUTION(NativePoissonDistribution, std::poisson_distribution, distribution_int_t, \
		(distribution_float_t)(mean)(1.0))

NATIVE_DISTRIBUTION(NativeExponentialDistribution, std::exponential_distribution, distribution_float_t, \
		(distribution_float_t)(lambda)(1.0))

// undefine Boost Preprocessor Macros
#define CLEANUP 1
#include "nativerandomdistributions_pp.h"
#undef CLEANUP

// TODO add more
//generate_canonical

//bernoulli_distribution

//negative_binomial_distribution

//geometric_distribution

//poisson_distribution

//exponential_distribution

//gamma_distribution

//weibull_distribution

//extreme_value_distribution

//normal_distribution

//lognormal_distribution

//chi_squared_distribution

//cauchy_distribution

//fisher_f_distribution

//student_t_distribution

//discrete_distribution

//piecewise_constant_distribution

//piecewise_linear_distribution
