#include "random.h"

#include <random>
#include <memory>
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT(RandomGenerator)
BOOST_CLASS_EXPORT_IMPLEMENT(RandomDistribution)

RandomGenerator::~RandomGenerator()
{}

RandomDistribution::RandomDistribution() :
    mConstrain(RandomDistribution::NONE)
{}

RandomDistribution::~RandomDistribution() 
{
}


void RandomDistribution::next(distribution_float_t &)
{
	throw std::runtime_error("Invalid value type");
}

void RandomDistribution::next(std::vector<distribution_float_t> &)
{
	throw std::runtime_error("Invalid value type");
}

void RandomDistribution::next(distribution_int_t &)
{
	throw std::runtime_error("Invalid value type");
}

void RandomDistribution::next(std::vector<distribution_int_t> &)
{
	throw std::runtime_error("Invalid value type");
}

void RandomDistribution::setBoundaries(ConstrainType c, distribution_int_t a, distribution_int_t b)
{
	mConstrain = c;
	setBoundaries(a, b);
}

void RandomDistribution::setBoundaries(ConstrainType c, distribution_float_t a, distribution_float_t b)
{
	mConstrain = c;
	setBoundaries(a, b);
}

void RandomDistribution::setBoundaries(distribution_int_t, distribution_int_t)
{
	throw std::runtime_error("Invalid value type");
}

void RandomDistribution::setBoundaries(distribution_float_t, distribution_float_t)
{
	throw std::runtime_error("Invalid value type");
}
