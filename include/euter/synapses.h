#pragma once

#include "parameter.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <typeinfo>

namespace euter {

struct GetterVisitor
{
	void operator()(double p) const
	{
		mP = p;
	}

	double extract() const
	{
		return mP;
	}

	mutable double mP;
};

} // namespace euter

#ifndef PYPLUSPLUS
#define HASH_FUN() \
	size_t hash() const                                                        \
	{                                                                          \
		return mParams.hash();                                                 \
	}
#else
#define HASH_FUN()
#endif

#define SERIALIZABLE_PARAMETERIZED_STRUCT(CLASS_NAME, BASE_NAME, ...)          \
namespace euter {                                                              \
struct CLASS_NAME : BASE_NAME                                                  \
{                                                                              \
	PARAMETER_STRUCT(Parameters, __VA_ARGS__)                                  \
	CLASS_NAME() : mParams() {}                                                \
	CLASS_NAME(CLASS_NAME::Parameters params) : mParams(params) {}             \
	                                                                           \
	bool operator==(CLASS_NAME const& other) const                             \
	{                                                                          \
		return (other.mParams == mParams);                                     \
	}                                                                          \
                           	                                                   \
	bool operator==(BASE_NAME const & other) const                             \
	{                                                                          \
		try {                                                                  \
			CLASS_NAME const & casted_other =                                  \
				dynamic_cast<CLASS_NAME const &>(other);                       \
			return (casted_other == *this);                                    \
		} catch (std::bad_cast & e) {                                          \
			return false;                                                      \
		}                                                                      \
	}                                                                          \
                                                                               \
	double get(std::string p) const                                            \
	{                                                                          \
		GetterVisitor getter{};                                                  \
		mParams.get(p, getter);                                                \
		return getter.extract();                                               \
	}                                                                          \
                                                                               \
	HASH_FUN()                                                                 \
	                                                                           \
	CLASS_NAME::Parameters mParams;                                            \
private:                                                                       \
	friend class boost::serialization::access;                                 \
	template<class Archive>                                                    \
	void serialize(Archive& ar, const unsigned int&/* version*/)               \
	{                                                                          \
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BASE_NAME)                    \
		   & BOOST_SERIALIZATION_NVP(mParams);                                 \
	}                                                                          \
};                                                                             \
} /* namespace euter */                                                        \
BOOST_CLASS_EXPORT_KEY(euter::CLASS_NAME)

namespace euter {

struct ShortTermPlasticityMechanism
{
	virtual ~ShortTermPlasticityMechanism() {}
	virtual double get(std::string) const { return 0; };
	virtual bool operator==(ShortTermPlasticityMechanism const&) const
	{
		return true;
	}                                                                          
#ifndef PYPLUSPLUS
	virtual size_t hash() const { return 0; };
#endif
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive&/* ar*/, const unsigned int&/* version*/)
	{
	}  
};

} // namespace euter

#ifndef PYPLUSPLUS
namespace std {
	template <>
	struct hash<euter::ShortTermPlasticityMechanism>
	{
		typedef euter::ShortTermPlasticityMechanism argument_type;
		typedef std::size_t  result_type;

		result_type operator()(euter::ShortTermPlasticityMechanism const & t) const
		{
			return t.hash();
		}
	};
}
#endif


SERIALIZABLE_PARAMETERIZED_STRUCT(TsodyksMarkramMechanism, ShortTermPlasticityMechanism,
	PARAMETER(double, U, 0.5),
	PARAMETER(double, tau_rec, 100.0),
	PARAMETER(double, tau_facil, 0.0),
	PARAMETER(double, u0, 0.0),
	PARAMETER(double, x0, 1.0),
	PARAMETER(double, y0, 0.0)
	)

namespace euter {

struct STDPWeightDependence
{
	virtual ~STDPWeightDependence() {}
	virtual double get(std::string p) const = 0;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive&/* ar*/, const unsigned int&/* version*/)
	{
	}  
};

} // namespace euter

SERIALIZABLE_PARAMETERIZED_STRUCT(AdditiveWeightDependence, STDPWeightDependence,
	PARAMETER(double, w_min, 0.0),
	PARAMETER(double, w_max, 1.0),
	PARAMETER(double, A_plus, 0.01),
	PARAMETER(double, A_minus, 0.01)
	)

SERIALIZABLE_PARAMETERIZED_STRUCT(MultiplicativeWeightDependence, STDPWeightDependence,
	PARAMETER(double, w_min, 0.0),
	PARAMETER(double, w_max, 1.0),
	PARAMETER(double, A_plus, 0.01),
	PARAMETER(double, A_minus, 0.01)
	)

SERIALIZABLE_PARAMETERIZED_STRUCT(AdditivePotentiationMultiplicativeDepression, STDPWeightDependence,
	PARAMETER(double, w_min, 0.0),
	PARAMETER(double, w_max, 1.0),
	PARAMETER(double, A_plus, 0.01),
	PARAMETER(double, A_minus, 0.01)
	)

SERIALIZABLE_PARAMETERIZED_STRUCT(GutigWeightDependence, STDPWeightDependence,
	PARAMETER(double, w_min, 0.0),
	PARAMETER(double, w_max, 1.0),
	PARAMETER(double, A_plus, 0.01),
	PARAMETER(double, A_minus, 0.01),
	PARAMETER(double, mu_plus, 0.5),
	PARAMETER(double, mu_minus, 0.5)
	)

namespace euter {

struct STDPTimingDependence
{
	virtual ~STDPTimingDependence() {}
	virtual double get(std::string p) const = 0;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive&/* ar*/, const unsigned int&/* version*/)
	{
	}  
};

} // namespace euter

SERIALIZABLE_PARAMETERIZED_STRUCT(SpikePairRule, STDPTimingDependence,
	PARAMETER(double, tau_plus, 20.0),
	PARAMETER(double, tau_minus, 20.0)
	)

namespace euter {

struct STDPMechanism
{
	STDPMechanism(
			double dendritic_delay_fraction = 1.0) :
		mDendriticDelayFraction(dendritic_delay_fraction) {}
	STDPMechanism(
			const boost::shared_ptr<STDPTimingDependence> timing_dependence,
			double dendritic_delay_fraction = 1.0) :
		mTimingDependence(timing_dependence),
		mDendriticDelayFraction(dendritic_delay_fraction) {}
	STDPMechanism(
			const boost::shared_ptr<STDPWeightDependence> weight_dependence,
			double dendritic_delay_fraction = 1.0) :
		mWeightDependence(weight_dependence),
		mDendriticDelayFraction(dendritic_delay_fraction) {}
	STDPMechanism(
			const boost::shared_ptr<STDPTimingDependence> timing_dependence,
			const boost::shared_ptr<STDPWeightDependence> weight_dependence,
			double dendritic_delay_fraction = 1.0) :
		mTimingDependence(timing_dependence),
		mWeightDependence(weight_dependence),
		mDendriticDelayFraction(dendritic_delay_fraction)
	{
	}

	boost::shared_ptr<STDPTimingDependence> mTimingDependence;
	boost::shared_ptr<STDPWeightDependence> mWeightDependence;
	double mDendriticDelayFraction;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(mTimingDependence)
		   & BOOST_SERIALIZATION_NVP(mWeightDependence)
		   & BOOST_SERIALIZATION_NVP(mDendriticDelayFraction);
	}  
};

struct SynapseDynamics
{
	SynapseDynamics(
			boost::shared_ptr<ShortTermPlasticityMechanism> fast = boost::make_shared<ShortTermPlasticityMechanism>(),
			boost::shared_ptr<STDPMechanism> slow = boost::make_shared<STDPMechanism>()
			) : mFast(fast), mSlow(slow) {}
	SynapseDynamics(
			boost::shared_ptr<STDPMechanism> slow
			) : mFast(boost::make_shared<TsodyksMarkramMechanism>()), mSlow(slow) {}

	boost::shared_ptr<ShortTermPlasticityMechanism> fast();
	boost::shared_ptr<const ShortTermPlasticityMechanism> fast() const;
	boost::shared_ptr<STDPMechanism> slow();
	boost::shared_ptr<const STDPMechanism> slow() const;
private:
	boost::shared_ptr<ShortTermPlasticityMechanism> mFast;
	boost::shared_ptr<STDPMechanism> mSlow;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int&/* version*/)
	{
		ar & BOOST_SERIALIZATION_NVP(mFast)
		   & BOOST_SERIALIZATION_NVP(mSlow);
	}
};

} // namespace euter
