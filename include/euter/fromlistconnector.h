#pragma once

#include "connector.h"

namespace euter {

class FromListConnector : public Connector
{
public:
	struct Connection
	{
		size_t from, to;
		template<typename Archiver>
		void serialize(Archiver& ar, const unsigned int& /*version*/);
	};

	typedef std::vector<Connection> Connections;

	FromListConnector(Connections && connections,
	                  const default_type & weights = 0.0,
	                  const default_type & delays  = 0.0);

	virtual void printOn(std::ostream &) const;

	virtual size_t connect(
	        const Assembly & pre,
	        const Assembly & post,
	        RandomGenerator & rnd,
	        matrix_type & matrix) const;

private:
	FromListConnector();
	std::vector<Connection> mConnections;

	friend class boost::serialization::access;
	template<typename Archiver>
		void serialize(Archiver& ar, const unsigned int&) {
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Connector)
			   & BOOST_SERIALIZATION_NVP(mConnections);
		}
};

template<typename Archiver>
inline void FromListConnector::Connection::serialize(Archiver& ar, const unsigned int& /*version*/)
{
	ar & BOOST_SERIALIZATION_NVP(from)
	   & BOOST_SERIALIZATION_NVP(to);
}

} // namespace euter

BOOST_CLASS_EXPORT_KEY(euter::FromListConnector)
