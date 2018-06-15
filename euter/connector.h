#pragma once

#include <boost/utility.hpp>
#include <boost/variant.hpp>
#include <vector>

#include "connector_types.h"

#include "assembly.h"

class RandomGenerator;

class Connector : public ConnectorTypes
{
public:
	Connector(const default_type & weights = 0.0,
	          const default_type & delays = 0.0);
	virtual ~Connector() {}

	/// Initilizes the matrix with zeros for existing connections and NaNs for
	/// not existing connections
	/// @return no of realised connections
	size_t initMatrix(Assembly & pre,
	                  Assembly & post,
	                  matrix_type & matrix,
	                  RandomGenerator & rnd) const;

	const default_type & getDefaultDelays() const;
	const default_type & getDefaultWeights() const;

	virtual void printOn(std::ostream &) const = 0;

private:
	/// This function is called to initilize the connection matrix
	/// The matrix values must be set to NaN for not existing connections and
	/// otherwise to 0.0
	/// @return no of realised connections
	virtual size_t connect(
	        const Assembly & pre,
	        const Assembly & post,
	        RandomGenerator & rnd,
	        matrix_type & matrix) const = 0;

	void zeroMatrix(matrix_type& matrix, size_t size1, size_t size2) const;

	default_type mWeights; // This are default values
	default_type mDelays;  // This are default values

	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int&) {
		ar & BOOST_SERIALIZATION_NVP(mWeights)
		   & BOOST_SERIALIZATION_NVP(mDelays);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Connector)
BOOST_CLASS_EXPORT_KEY(Connector)

// TODO: Implement class Space first!
//class DistanceDependentProbabilityConnector : public Connector
//{};
