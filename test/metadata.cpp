#include <gtest/gtest.h>
#include <sstream>

#include "euter/metadata.h"

using namespace euter;

// Example metadata class :)
struct X : DerivedMetaData<X>
{
	std::string name() const { return "X"; }

private:
	friend class boost::serialization::access;

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(data);
	}

	int data;
};

TEST(MetaData, Serialization)
{
	X x, y;

	std::stringstream sstr;

	{
		boost::archive::xml_oarchive ar(sstr);
		ar << BOOST_SERIALIZATION_NVP(x);
	}

	sstr.flush();

	{
		boost::archive::xml_iarchive ar(sstr);
		ar >> BOOST_SERIALIZATION_NVP(y);
	}
}
