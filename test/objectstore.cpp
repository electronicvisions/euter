#include <gtest/gtest.h>

#include "euter/objectstore.h"

using testing::StaticAssertTypeEq;

TEST(Objectstore, Reset) {
	ObjectStore store;

	const size_t npop = 5;

	auto p1 = Population::create(store, npop, CellType::IF_brainscales_hardware);

	for(size_t pop = 0; pop != npop; ++pop) {
		p1->getSpikes(pop).push_back(0);
	}

	for(size_t pop = 0; pop != npop; ++pop) {
		ASSERT_EQ(p1->getSpikes(pop).size(), 1);
	}

	store.reset();

	for(size_t pop = 0; pop != npop; ++pop) {
		ASSERT_EQ(p1->getSpikes(pop).size(), 0);
	}

}
