#include <tuple>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iostream>

#include <gtest/gtest.h>

#include "boost/serialization/serialization.h"

using boost::serialization::make_array;

typedef std::tuple<boost::archive::xml_iarchive, boost::archive::xml_oarchive> XMLArchives;
typedef std::tuple<boost::archive::text_iarchive, boost::archive::text_oarchive> TextArchives;
typedef std::tuple<boost::archive::binary_iarchive, boost::archive::binary_oarchive> BinaryArchives;

typedef ::testing::Types<XMLArchives, TextArchives, BinaryArchives> ArchiveTypes;

template <typename T>
class SerializationTest;

template <typename IArchive, typename OArchive>
class SerializationTest<std::tuple<IArchive, OArchive> > : public ::testing::Test
{
public:
	SerializationTest() : ss(), oa(nullptr), ia(nullptr) {}

	virtual void SetUp() {
		oa.reset(new OArchive(ss));
		ia.reset(new IArchive(ss));
	}

	void serialize(bool with_nan = false) {
		for (unsigned ii = 0; ii < size; ++ ii) {
			in[ii] = 3 * ii + 2.5;
			if (with_nan) {
				int const n = rand();
				if (n>std::numeric_limits<int>::max()/2) {
					in[ii] = std::numeric_limits<double>::quiet_NaN();
				}
			}
		}
		reset();
		*oa.get() << make_nvp("object", make_array(in, size));
		flush();
	}

	void deserialize() {
		double out[size];
		*ia.get() >> make_nvp("object", make_array(out, size));
		for (unsigned ii = 0; ii < size; ++ii) {
			if (std::isnan(in[ii]) && std::isnan(out[ii]))
				continue;
			EXPECT_FLOAT_EQ(in[ii], out[ii]);
		}
	}

private:
	void reset() {
		ss.str("");
		ss.clear();
	}

	void flush() {
		ss.flush();
	}

	enum : size_t { size = 430 };
	double in[size];
	std::stringstream ss;
	std::unique_ptr<OArchive> oa;
	std::unique_ptr<IArchive> ia;
};

TYPED_TEST_CASE(SerializationTest, ArchiveTypes);

TYPED_TEST(SerializationTest, MakeArray)
{
	this->serialize();
	this->deserialize();
}

template <class T>
class SerializationTest2 : public SerializationTest<T> {
};

TYPED_TEST_CASE_P(SerializationTest2);

TYPED_TEST_P(SerializationTest2, MakeArrayWithNonPrintableData) {
	this->serialize(true);
	this->deserialize();
}

REGISTER_TYPED_TEST_CASE_P(SerializationTest2, MakeArrayWithNonPrintableData);

INSTANTIATE_TYPED_TEST_CASE_P(SerializationTestBinaryOnly, // Instance name
                              SerializationTest2, // Test case name
                              BinaryArchives); // Type list
