#include "euter/fixednumberpostconnector.h"

#include <numeric>

#include <log4cxx/logger.h>

#include "euter/exceptions.h"
#include "euter/assembly.h"

FixedNumberPostConnector::FixedNumberPostConnector() :
    mN(0),
    mAllowSelfConnections(true)
{} // for serialization

FixedNumberPostConnector::FixedNumberPostConnector(
	const size_t n, // TODO: support RandomDistribution
	const bool allow_self_connections,
	const default_type & weights,
	const default_type & delays) :
    Connector(weights, delays),
    mN(n),
    mAllowSelfConnections(allow_self_connections)
{}

size_t FixedNumberPostConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & rnd,
        matrix_type & matrix) const
{
	if(mN > (post.size() - int(!mAllowSelfConnections)))
		LOG4CXX_WARN(log4cxx::Logger::getLogger("PyHMF"), "Multiple connections currently not supported! You may use multiple connectors instead.");
    
	size_t elements = 0;
    std::vector<size_t> pool(post.size());
    std::iota(pool.begin(), pool.end(), 0);

    for(size_t ii=0; ii<matrix.size1(); ii++)
    {
        matrix_row_type row(matrix, ii);
        std::fill(row.begin(), row.end(), std::numeric_limits<double>::quiet_NaN());
		
        auto p = rnd.permutation(pool);
		if(!mAllowSelfConnections && pre == post)
			p.erase(std::find(p.begin(), p.end(), ii));

        for(auto c=0u; c<std::min(mN, post.size()-int(!mAllowSelfConnections)); c++)
        {
            row(p[c]) = 0.0;
            ++elements;
        }
    }
    return elements;
}

void FixedNumberPostConnector::printOn(std::ostream& o) const
{
	o << "FixedNumberPostConnector";
}

BOOST_CLASS_EXPORT_IMPLEMENT(FixedNumberPostConnector)
