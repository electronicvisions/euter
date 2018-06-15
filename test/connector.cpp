#include <gtest/gtest.h>
#include <boost/make_shared.hpp>

#include "euter/alltoallconnector.h"
#include "euter/connector.h"
#include "euter/fixednumberpreconnector.h"
#include "euter/fixedprobabilityconnector.h"
#include "euter/fromlistconnector.h"
#include "euter/nativerandomgenerator.h"
#include "euter/onetooneconnector.h"
#include "euter/projection.h"
#include "euter/objectstore.h"

using boost::make_shared;

class Connectors : public ::testing::Test
{
public:
	Connectors() :
	    rng(new NativeRandomGenerator(22949142706))
	{}

protected:
	boost::shared_ptr<RandomGenerator> rng;
	boost::shared_ptr<RandomGenerator> & getRng() { return rng; }
	ObjectStore store;
};

TEST_F(Connectors, AllToAllConnector) {
	auto p10   = Population::create(store, 10, CellType::IF_brainscales_hardware);
	auto p20   = Population::create(store, 20, CellType::IF_brainscales_hardware);

	auto proj = Projection::create(store, p10, p20,
		make_shared<AllToAllConnector>(), getRng());

	ASSERT_NO_THROW(proj->getWeights());

	ProjectionMatrix weights = proj->getWeights();
	EXPECT_EQ(p10->size(), weights.get().size1());
	EXPECT_EQ(p20->size(), weights.get().size2());
}


TEST_F(Connectors, OneToOneConnector) {
	auto p10   = Population::create(store, 10, CellType::IF_brainscales_hardware);
	auto p10_2 = Population::create(store, 10, CellType::IF_brainscales_hardware);
	auto p20   = Population::create(store, 20, CellType::IF_brainscales_hardware);

	ASSERT_THROW(auto proj = Projection::create(store,
	            p10, p20, make_shared<OneToOneConnector>(),
	            getRng()), std::length_error);

	auto proj2 = Projection::create(store, p10, p10_2,
		make_shared<OneToOneConnector>(), getRng());

	ASSERT_NO_THROW(proj2->getWeights());

	ProjectionMatrix weights = proj2->getWeights();
	EXPECT_EQ(p10->size(), weights.get().size1());
	EXPECT_EQ(p10_2->size(), weights.get().size2());
}


TEST_F(Connectors, FixedProbabilityConnector) {
	auto p500 = Population::create(store, 500, CellType::IF_brainscales_hardware);
	auto p200 = Population::create(store, 200, CellType::IF_brainscales_hardware);

	double prob   = 0.5;
	double weight = 5;
	double delay  = 5;

	auto c = make_shared<FixedProbabilityConnector>(prob, true, weight, delay);
	auto proj = Projection::create(store, p500, p200, c, getRng());

	ASSERT_NO_THROW(proj->getWeights());

	ProjectionMatrix weights = proj->getWeights();
	EXPECT_EQ(p500->size(), weights.get().size1());
	EXPECT_EQ(p200->size(), weights.get().size2());
}

