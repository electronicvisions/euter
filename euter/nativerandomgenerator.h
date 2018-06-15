#pragma once

#include <algorithm>
#include <random>
#include <string>

#include "random.h"

// TODO we could template it to support more native rngs
class NativeRandomGenerator : public RandomGenerator
{
public:
	typedef std::mt19937 rng_type;
	static_assert(std::is_same<rng_type::result_type, result_type>::value, "");

	NativeRandomGenerator() {}
	NativeRandomGenerator(result_type seed) : mRng(seed) {}

    virtual result_type next()
	{ return mRng(); }

	virtual std::vector<result_type> next(result_type n)
	{
		std::vector<result_type> values(n);
		std::generate_n(values.begin(), n, mRng);
		return values;
	}

	virtual size_t seed() const
	{ return mSeed; }

	virtual void seed(result_type seed)
	{ mSeed = seed; mRng.seed(mSeed); }

	virtual bool serializable()
	{ return true; }

	virtual result_type min() const
	{ return mRng.min(); }

	virtual result_type max() const
	{ return mRng.max(); }

	RandomGenerator* save() const
	{ return new NativeRandomGenerator(*this); }

	rng_type & raw()
	{ return mRng; }

private:
	result_type mSeed;
	rng_type mRng;

	std::string getState() const;
	void setState(const std::string & state);

	template<typename Archiver>
	void save(Archiver& ar, const unsigned int & version) const;
	template<typename Archiver>
	void load(Archiver& ar, const unsigned int & version);

	BOOST_SERIALIZATION_SPLIT_MEMBER()

	friend class boost::serialization::access;
};

template<typename Archiver>
void NativeRandomGenerator::save(Archiver& ar, const unsigned int&) const
{
	std::string state = getState();
	ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(RandomGenerator)
	   << BOOST_SERIALIZATION_NVP(mSeed)
	   << BOOST_SERIALIZATION_NVP(state);
}
template<typename Archiver>
void NativeRandomGenerator::load(Archiver& ar, const unsigned int& /*version*/)
{
	std::string state;
	ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(RandomGenerator)
	   >> BOOST_SERIALIZATION_NVP(mSeed)
	   >> BOOST_SERIALIZATION_NVP(state);
	setState(state);
}

BOOST_CLASS_EXPORT_KEY(NativeRandomGenerator)

// TODO
// #include "boost_python_fwd.h"
//class PythonRandomGenerator
//{
//public:
//	PythonRandomGenerator(bp::object obj);

//	typedef uint_fast32_t result_type;

//	explicit PythonRandomGenerator(result_type x0 = default_seed);
//	PythonRandomGenerator(PythonRandomGenerator & right);
//	PythonRandomGenerator(const PythonRandomGenerator & right);

//	void seed(result_type x0 = default_seed);

//	result_type min() const;
//	result_type max() const;
//	result_type operator()();
//private:
//	bp::object obj;

//	static constexpr result_type default_seed = 1u;
//};

//class GSLRandomGenerator
//{
//public:

//	virtual double next() = 0;
//	virtual std::vector<double> next(size_t n) = 0;
//	virtual size_t seed() const = 0;
//	virtual void seed(size_t seed) = 0;

//private:
//	std::unique_ptr<bp::object> mRng;
//}
