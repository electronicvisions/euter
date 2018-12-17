#include <gtest/gtest.h>

#include <numeric>

#include "euter/objectstore.h"
#include "euter/projection.h"

using namespace euter;
using testing::StaticAssertTypeEq;

TEST(Population, BasicCheck) {
	ObjectStore store;
	ASSERT_THROW(Population::create(store, 0, CellType::IF_brainscales_hardware), std::runtime_error);

	auto p1 =   Population::create(store, 1, CellType::IF_brainscales_hardware);
	auto p500 = Population::create(store, 500, CellType::IF_brainscales_hardware);
	auto p200 = Population::create(store, 200, CellType::IF_brainscales_hardware);
}


TEST(Population, NeuronEnumeration) {
	ObjectStore store;
	Population::create(store, 2, CellType::IF_brainscales_hardware);
	Population::create(store, 3, CellType::IF_brainscales_hardware);
	Population::create(store, 5, CellType::IF_brainscales_hardware);
	Population::create(store, 7, CellType::IF_brainscales_hardware);
	Population::create(store, 11, CellType::IF_brainscales_hardware);
	Population::create(store, 13, CellType::IF_brainscales_hardware);
	Population::create(store, 17, CellType::IF_brainscales_hardware);

	std::vector<size_t> indices;
	for (auto p : store.populations())
	{
		for(size_t ii = 0; ii < p->size(); ++ii)
			indices.push_back(p->globalNeuronId(ii));
	}

	std::vector<size_t> ref(2+3+5+7+11+13+17);
	std::iota(ref.begin(), ref.end(), 0);

	ASSERT_EQ(ref, indices);
}
