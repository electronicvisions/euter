#include "onetooneconnector.h"
#include "assembly.h"

OneToOneConnector::OneToOneConnector(
        const bool allow_self_connections,
        const default_type & weights,
        const default_type & delays ) :
	Connector(weights, delays),
	mAllowSelfConnections(allow_self_connections)
{}

size_t OneToOneConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & ,
        matrix_type & matrix) const
{
	if (pre.size() != post.size())
	{
		throw std::length_error("pre.size() != post.size()"); 		// TODO: add testcase for this
	}
	size_t size = pre.size();
	matrix = ublas::scalar_matrix<value_type>(
	            size,
	            size,
	            std::numeric_limits<double>::quiet_NaN());
	for (size_t ii = 0; ii < size; ii++)
	{
		matrix(ii, ii) = 0.0;
	}
	return size;
}

void OneToOneConnector::printOn(std::ostream& o) const
{
	o << "OneToOneConnector";
}

BOOST_CLASS_EXPORT_IMPLEMENT(OneToOneConnector)
