#include "fixedprobabilityconnector.h"

#include <random>
#include "random.h"
#include "random_traits.h"

FixedProbabilityConnector::FixedProbabilityConnector(
        double       const p_connect,
        bool         const allow_self_connections,
        const default_type & weights,
        const default_type & delays ) :
	Connector(weights, delays),
	mP(p_connect),
	mAllowSelfConnections(allow_self_connections)
{}


std::random_device rd;
std::mt19937 gen(rd());

size_t FixedProbabilityConnector::connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & rnd,
        matrix_type & matrix) const
{
	size_t elements = 0;
	std::uniform_real_distribution<distribution_float_t> dis(0, 1);

	auto it = matrix.data().begin();
	auto iend = matrix.data().end();
	for(; it != iend; ++it)
	{
		if ((mP > dis(rnd))
				&& !(!mAllowSelfConnections
					&& pre == post
					&& (it - matrix.data().begin()) % (matrix.size2() + 1) == 0))
		{
			*it = 0.0;
			++elements;
		}
		else
		{
			*it = std::numeric_limits<double>::quiet_NaN();
		}
	}
	return elements;
}

void FixedProbabilityConnector::printOn(std::ostream& o) const
{
	o << "FixedProbabilityConnector";
}

BOOST_CLASS_EXPORT_IMPLEMENT(FixedProbabilityConnector)
