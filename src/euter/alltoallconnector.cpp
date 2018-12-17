#include "euter/alltoallconnector.h"

void AllToAllConnector::printOn(std::ostream& o) const
{
	o << "AllToAllConnector";
}

/* AllToAllConnector */
AllToAllConnector::AllToAllConnector(
        const bool allow_self_connections,
        const default_type & weights,
        const default_type & delays) :
    Connector(weights, delays),
    mAllowSelfConnections(allow_self_connections)
{}

size_t AllToAllConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator &/* rng*/,
        matrix_type & matrix) const
{
	if(!mAllowSelfConnections && pre == post)
	{
		for(size_t i=0; i<pre.size(); i++)
		{
			matrix(i, i) = std::numeric_limits<double>::quiet_NaN();
		}
		return matrix.data().size() - pre.size();
	}
	else
	{
		return matrix.size1() * matrix.size2();
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(AllToAllConnector)
