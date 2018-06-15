#pragma once

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "boost/serialization/serialization.h"

#include "euter/class_name.h"

class MetaData;
typedef boost::shared_ptr<MetaData> MetaDataPtr;

class MetaData
{
public:
	virtual ~MetaData() {}
	// Name of the keyword argument refering to the metadata
	virtual std::string name() const = 0;
	virtual std::string toString() const = 0;
	virtual void fromString(std::string const& str) = 0;

	virtual MetaData& operator= (MetaData const&)
	{
		return *this;
	}
};

template <typename T>
class DerivedMetaData : public MetaData
{
public:
	typedef DerivedMetaData<T> base_t;

	virtual std::string name() const
	{
		return class_name<T>();
	}

	virtual std::string toString() const
	{
#ifndef PYPLUSPLUS
		T const& metadata = dynamic_cast<T const&>(*this);
		std::string name = this->name();

		std::stringstream ar;
		boost::archive::xml_oarchive oa(ar);
		oa << BOOST_SERIALIZATION_NVP(name);
		oa << boost::serialization::make_nvp(name.c_str(), metadata);
		return ar.str();
#else
		return std::string();
#endif
	}

	virtual void fromString(std::string const& str)
	{
#ifndef PYPLUSPLUS
		T & metadata = dynamic_cast<T&>(*this);
        std::string name;

		std::stringstream tmp(str);
		boost::archive::xml_iarchive ia(tmp);
		ia >> BOOST_SERIALIZATION_NVP(name);

		if (name != this->name())
		{
			throw std::runtime_error("Invalid MetaData entry for " + name);
		}

		ia >> boost::serialization::make_nvp(name.c_str(), metadata);
#endif
	}

	virtual MetaData& operator= (MetaData const& m)
	{
		*this = dynamic_cast<T const&>(m);
		return *this;
	}
};
