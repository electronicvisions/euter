#pragma once

#include "connector.h"

namespace euter {

class FixedNumberPostConnector : public Connector
{
public:
	FixedNumberPostConnector();
	FixedNumberPostConnector(
		const size_t n, // TODO: support RandomDistribution
		const bool allow_self_connections = true,
		const default_type & weight = 0.0,
		const default_type & delay  = 0.0
	);

	virtual void printOn(std::ostream &) const;

	virtual size_t connect(
	        const Assembly & pre,
	        const Assembly & post,
	        RandomGenerator & rnd,
	        matrix_type & matrix) const;

private:
	size_t mN;
	bool   mAllowSelfConnections;

	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int&) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Connector)
		   & BOOST_SERIALIZATION_NVP(mN)
		   & BOOST_SERIALIZATION_NVP(mAllowSelfConnections);
	}
};

} // namespace euter

BOOST_CLASS_EXPORT_KEY(euter::FixedNumberPostConnector)
