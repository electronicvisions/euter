#include <iterator>
#include <gtest/gtest.h>

template <typename T>
struct TestEqual
{
	typedef T value_type;
	bool operator()(const value_type v1, const value_type v2)
	{
		return v1 == v2;
	}
};

template <>
struct TestEqual<float>
{
	typedef float value_type;
	bool operator()(const value_type v1, const value_type v2)
	{
		return v1 == v2 || (std::isnan(v1) && std::isnan(v2));
	}
};

template <>
struct TestEqual<double>
{
	typedef double value_type;
	bool operator()(const value_type v1, const value_type v2)
	{
		return v1 == v2 || (std::isnan(v1) && std::isnan(v2));
	}
};

template<typename T>
::testing::AssertionResult ContainerEqual(const T & a, const T & b)
{
	size_t pos = 0;
	TestEqual<typename T::value_type> eq;

	if (a.size() != b.size())
	{
		return ::testing::AssertionFailure()
		        << "Mismatch in container size got: "
		        << a.size()
		        << " expected: "
		        << b.size();
	}


	auto iter_a = a.begin();
	auto iend_a = b.end();
	auto iter_b = b.begin();
	auto iend_b = b.end();

	while(iter_a != iend_a && iter_b != iend_b)
	{
		if ( !eq(*iter_a, *iter_b))
		{
			return ::testing::AssertionFailure()
			        << "Mismatch at position " << pos
			        << " found '" << *iter_a
			        << "' expected '" << *iter_b
			        << "'";
		}
		++iter_a; ++iter_b; ++pos;
	}

	return ::testing::AssertionSuccess();
}

