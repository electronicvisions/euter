#pragma once

#include "boost/serialization/serialization.h"

struct Point3d
{
	double x,y,z;

	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int& /*version*/);
};


template<typename Archiver>
inline void Point3d::serialize(Archiver& ar, const unsigned int& /*version*/)
{
	ar & BOOST_SERIALIZATION_NVP(x)
	   & BOOST_SERIALIZATION_NVP(y)
	   & BOOST_SERIALIZATION_NVP(z);
}
