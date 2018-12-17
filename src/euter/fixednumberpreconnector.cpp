#include "euter/fixednumberpreconnector.h"

#include <numeric>

#include <log4cxx/logger.h>

#include "euter/exceptions.h"
#include "euter/assembly.h"

FixedNumberPreConnector::FixedNumberPreConnector() :
    mN(0),
    mAllowSelfConnections(true)
{} // for serialization

FixedNumberPreConnector::FixedNumberPreConnector(
	const size_t n, // TODO: support RandomDistribution
	const bool allow_self_connections,
	const default_type & weights,
	const default_type & delays) :
    Connector(weights, delays),
    mN(n),
    mAllowSelfConnections(allow_self_connections)
{}

size_t FixedNumberPreConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & rnd,
        matrix_type & matrix) const
{
	if(mN > (pre.size() - int(!mAllowSelfConnections)))
		LOG4CXX_WARN(log4cxx::Logger::getLogger("PyHMF"), "Multiple connections currently not supported! You may use multiple connectors instead.");
    
	size_t elements = 0;
    std::vector<size_t> pool(pre.size());
    std::iota(pool.begin(), pool.end(), 0);

    for(size_t ii=0; ii<matrix.size2(); ii++)
    {
        matrix_col_type col(matrix, ii);
        std::fill(col.begin(), col.end(), std::numeric_limits<double>::quiet_NaN());
		
        auto p = rnd.permutation(pool);
		if(!mAllowSelfConnections && pre == post)
			p.erase(std::find(p.begin(), p.end(), ii));

        for(auto c=0u; c<std::min(mN, pre.size()-int(!mAllowSelfConnections)); c++)
        {
            col(p[c]) = 0.0;
            ++elements;
        }
    }
	/*
    for(size_t ii=0; ii<matrix.size1(); ii++)
    {
        matrix_col_type col(matrix, ii);
        std::fill(col.begin(), col.end(), std::numeric_limits<double>::quiet_NaN());

        auto p = rnd.permutation(pool);
        for(auto c=0u; c<mN; c++)
        {
            col(p[c]) = 0.0;
            ++elements;
        }
    }
	*/
    return elements;
}

void FixedNumberPreConnector::printOn(std::ostream& o) const
{
	o << "FixedNumberPreConnector";
}

BOOST_CLASS_EXPORT_IMPLEMENT(FixedNumberPreConnector)
