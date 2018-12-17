#pragma once

#include <algorithm>

#include "boost/serialization/serialization.h"
#include "random_traits.h"

/// Abstract base for RandomGenerators to be use.
/// This class implements the interface used by the stl random engines to allow
/// easy usage with stl distributions
class RandomGenerator
{
public:
	typedef random_int_t result_type;

	virtual ~RandomGenerator();
	virtual result_type next() = 0;
	virtual std::vector<result_type> next(result_type n) = 0;
    
    template<typename T>
    std::vector<T> permutation(std::vector<T> v);

    template<typename T>
    void shuffle(std::vector<T> &v);

    template<typename T>
    std::vector<T> draw(std::vector<T> v, int n);

	virtual size_t seed() const = 0;
	virtual void seed(result_type seed) = 0;

	virtual bool serializable() = 0;

	virtual result_type min() const = 0;
	virtual result_type max() const = 0;

	// Returns a copy that will produce the same numbers again
	virtual RandomGenerator * save() const = 0;

	result_type operator()()
	{ return next(); }

private:
	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& , const unsigned int &) {}
};

template<typename T>
void RandomGenerator::shuffle(std::vector<T> &v)
{
    std::random_shuffle(v.begin(), v.end(), [this](unsigned n) { return next() % n; });
}

template<typename T>
std::vector<T> RandomGenerator::permutation(std::vector<T> v)
{
    shuffle(v);
    return v;
}

// This performs better and is more elegant than using permutation but due to
// compat issues with PyNN we do not really use it at the moment.
template<typename T>
std::vector<T> RandomGenerator::draw(std::vector<T> w, int n)
{
    using std::swap;
    for(auto i=w.begin(); i<w.begin()+n; ++i)
        swap(*i, i[next() % (w.end()-i)]);
    
    w.resize(n);
    return w;
}

class RandomDistribution
{
public:
	enum ResultType { REAL, INT };
	enum ConstrainType { NONE, CLIP, REDRAW };

	RandomDistribution();
	virtual ~RandomDistribution();

	virtual void next(distribution_float_t & v);
	virtual void next(std::vector<distribution_float_t> & v);
	virtual void next(distribution_int_t & v);
	virtual void next(std::vector<distribution_int_t> & v);

	void setBoundaries(ConstrainType c, distribution_int_t a, distribution_int_t b);
	void setBoundaries(ConstrainType c, distribution_float_t a, distribution_float_t b);

	virtual ResultType type() const = 0;
	virtual RandomDistribution * save() const = 0;

	RandomDistribution::ConstrainType constrain() const
	{ return mConstrain; }

private:
	RandomDistribution::ConstrainType mConstrain;

	virtual void setBoundaries(distribution_int_t a, distribution_int_t b);
	virtual void setBoundaries(distribution_float_t a, distribution_float_t b);

	template<typename Archiver>
	void save(Archiver& ar, const unsigned int &) const;
	template<typename Archiver>
	void load(Archiver& ar, const unsigned int &);

	BOOST_SERIALIZATION_SPLIT_MEMBER()
	friend class boost::serialization::access;
};

template<typename Archiver>
void RandomDistribution::save(Archiver& ar, const unsigned int &) const
{
	int ii = mConstrain;
	ar << BOOST_SERIALIZATION_NVP(ii);
}

template<typename Archiver>
void RandomDistribution::load(Archiver& ar, const unsigned int &)
{
	int ii;
	ar >> BOOST_SERIALIZATION_NVP(ii);
	switch(ii)
	{
	case RandomDistribution::CLIP:
		mConstrain = RandomDistribution::CLIP;
		break;
	case RandomDistribution::REDRAW:
		mConstrain = RandomDistribution::REDRAW;
		break;
	default:
		mConstrain = RandomDistribution::NONE;
	}
}

template  <typename T>
struct RandomDistributionTraits {};

template  <>
struct RandomDistributionTraits<distribution_int_t>
{
	static const RandomDistribution::ResultType resultType = RandomDistribution::INT;
};

template <>
struct RandomDistributionTraits<distribution_float_t>
{
	static const RandomDistribution::ResultType resultType = RandomDistribution::REAL;
};

template <typename T>
struct NumericRange
{
	NumericRange() {}
	NumericRange(T a , T b)
	{ set(a, b); }

	void set(T a , T b)
	{
		mMin = std::min(a, b);
		mMax = std::max(a, b);
	}

	bool test(const T v) const
	{ return ( v > mMin && v < mMax ); }

	void clip(T & v) const
	{ v = std::min(mMax, std::max(mMin, v)); }

	T mMin, mMax;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(RandomGenerator)
BOOST_SERIALIZATION_ASSUME_ABSTRACT(RandomDistribution)

BOOST_CLASS_EXPORT_KEY(RandomGenerator)
BOOST_CLASS_EXPORT_KEY(RandomDistribution)
