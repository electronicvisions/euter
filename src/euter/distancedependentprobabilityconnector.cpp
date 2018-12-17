#include "euter/distancedependentprobabilityconnector.h"

#include <numeric>
#include <random>

#include "euter/assembly.h"

DistanceDependentProbabilityConnector::DistanceDependentProbabilityConnector(
	boost::shared_ptr<ProbabilityGenerator> const& probability_generator,
	const bool allow_self_connections,
	const default_type& weights,
	const default_type& delays,
	const boost::shared_ptr<Space>& space,
	const int n_connections):
        Connector(weights, delays),
        mAllowSelfConnections(allow_self_connections),
        mProbabilityGenerator(probability_generator),
		mSpace(space),
		mNConnections(n_connections)
{}

size_t DistanceDependentProbabilityConnector::connect(
    const Assembly & pre,
    const Assembly & post,
    RandomGenerator & rnd,
    matrix_type & matrix) const
{
    std::uniform_real_distribution<distribution_float_t> dis(0, 1);

    auto prePositions = pre.positions();
    auto postPositions = post.positions();
	ublas::matrix<SpatialTypes::distance_type> distances =
	    mSpace->distances(prePositions, postPositions);
	ConnectorTypes::matrix_type probs = (*mProbabilityGenerator)(distances);

    std::vector<size_t> pool(post.size());
    std::iota(pool.begin(), pool.end(), 0);

	size_t count = 0;
	for (size_t i = 0; i < pre.size(); i++) {
		size_t row_count = 0;

		if (mNConnections != -1)
			rnd.shuffle(pool);

		for (size_t j = 0; j < pool.size(); j++) {
			if (probs(i, pool[j]) > dis(rnd) &&
			    !(!mAllowSelfConnections && pre == post && i == pool[j]) &&
			    (mNConnections == -1 || row_count < (unsigned)mNConnections)) {
				matrix(i, pool[j]) = 0.0;
				row_count++;
			} else {
				matrix(i, pool[j]) = std::numeric_limits<double>::quiet_NaN();
			}
		}
		count += row_count;
	}
	return count;
}

void DistanceDependentProbabilityConnector::printOn(std::ostream& o) const
{
    o << "DistanceDependentProbabilityConnector";
}

DistanceDependentProbabilityConnector::DistanceDependentProbabilityConnector() :
	mAllowSelfConnections(true),
	mProbabilityGenerator()
{}

BOOST_CLASS_EXPORT_IMPLEMENT(DistanceDependentProbabilityConnector)
