#pragma once

#include "connector.h"

namespace euter {

class OneToOneConnector : public Connector
{
public:
	OneToOneConnector(
	        const bool allow_self_connections = true,
	        const default_type & weights = 0.0,
	        const default_type & delays  = 0.0
	        );

	virtual void printOn(std::ostream &) const;

private:
	virtual size_t connect(
	        const Assembly & pre,
	        const Assembly & post,
	        RandomGenerator & rnd,
	        matrix_type & matrix) const;

	bool   mAllowSelfConnections;

	friend class boost::serialization::access;
	template<typename Archiver>
		void serialize(Archiver& ar, const unsigned int&) {
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Connector)
			   & BOOST_SERIALIZATION_NVP(mAllowSelfConnections);
		}
};

} // namespace euter

BOOST_CLASS_EXPORT_KEY(euter::OneToOneConnector)
