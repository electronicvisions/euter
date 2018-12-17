#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <limits>

#include <boost/make_shared.hpp>

#include "euter/connector.h"
#include "euter/population.h"

BOOST_CLASS_EXPORT_IMPLEMENT(euter::Connector)

namespace euter {

Connector::Connector(const default_type & weights,
                     const default_type & delays) :
    mWeights(weights),
    mDelays(delays)
{
}

size_t Connector::initMatrix(Assembly & pre,
                             Assembly & post,
                             matrix_type & matrix,
                             RandomGenerator & rnd) const
{
	zeroMatrix(matrix, pre.size(), post.size());

	size_t const n = connect(pre, post, rnd, matrix);

	if (matrix.size1() != pre.size() || matrix.size2() != post.size())
	{
		throw std::runtime_error("Invalid matrix size, the connector implementation seem to be wrong :(");
	}
	return n;
}

void Connector::zeroMatrix(matrix_type& matrix,
							size_t size1,
							size_t size2) const
{
	matrix.resize(size1, size2);
	for (auto& val : matrix.data())
		val = 0;
}

const Connector::default_type & Connector::getDefaultDelays() const
{
	return mDelays;
}

const Connector::default_type & Connector::getDefaultWeights() const
{
	return mWeights;
}

} // namespace euter
