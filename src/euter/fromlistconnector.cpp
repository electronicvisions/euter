#include "euter/fromlistconnector.h"
#include "euter/assembly.h"

#include <sstream>

namespace euter {

FromListConnector::FromListConnector()
{
}

FromListConnector::FromListConnector(std::vector<Connection> && connections,
                                     const default_type & weights,
                                     const default_type & delays) :
    Connector(weights, delays),
    mConnections(std::move(connections))
{
}

void FromListConnector::printOn(std::ostream& o) const
{
	o << "FromListConnector";
}

size_t FromListConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & ,
        matrix_type & matrix) const
{
	matrix = ublas::scalar_matrix<value_type>(
	            pre.size(),
	            post.size(),
	            std::numeric_limits<double>::quiet_NaN());

	for(const Connection & c : mConnections)
	{
		if (c.from >= pre.size() || c.to >= post.size())
		{
			std::stringstream err;
			err << "Invalid connection from "
			    << c.from << " to " << c.to << ".";
			throw std::runtime_error(err.str());
		}
		matrix(c.from, c.to) = 0.0;
	}
	return mConnections.size();
}

} // namespace euter

BOOST_CLASS_EXPORT_IMPLEMENT(euter::FromListConnector)
