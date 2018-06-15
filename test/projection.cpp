#include <iostream>
#include <gtest/gtest.h>
#include <boost/smart_ptr/make_shared.hpp>

#include "euter/fixedprobabilityconnector.h"
#include "euter/projection.h"
#include "euter/objectstore.h"
#include "euter/nativerandomgenerator.h"

using testing::StaticAssertTypeEq;
using boost::make_shared;

TEST(Projection, BasicCheck) {
}

TEST(Projection, GetWeights) {
	ObjectStore store;

	boost::shared_ptr<RandomGenerator> rng =
	        make_shared<NativeRandomGenerator>(22949142706);

	auto p500 = Population::create(store, 500, CellType::IF_brainscales_hardware);
	auto p200 = Population::create(store, 200, CellType::IF_brainscales_hardware);

	double prob   = 0.5;
	double weight = 5;
	double delay  = 5;

	auto proj = Projection::create(
		store, p500, p200,
		make_shared<FixedProbabilityConnector>(prob, true, weight, delay), rng);

	ASSERT_NO_THROW(proj->getWeights());

	Connector::matrix_type weights = proj->getWeights().get();
	EXPECT_EQ(p500->size(), weights.size1());
	EXPECT_EQ(p200->size(), weights.size2());
}
