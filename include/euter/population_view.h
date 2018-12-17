#pragma once

#include <iosfwd>

#include "boost/serialization/serialization.h"
#include "boost/serialization/dynamic_bitset.h"
#include "population.h"

#include <boost/weak_ptr.hpp>

class PopulationView
{
public:
	typedef boost::dynamic_bitset<> mask_type;

	PopulationView();
	PopulationView(PopulationView const&) = default;
	PopulationView(PopulationPtr pop);
	PopulationView(PopulationPtr pop, mask_type const& mask);

	// dirty hack, implement operator= via inplace new to allow PopulationViews
	// in STL containers despite all const members.
	PopulationView& operator=(PopulationView const& other);

	PopulationView copy_with_mask(mask_type const& mask) const;

	mask_type const& mask() const;

	Population&       population();
	Population const& population() const;

	PopulationPtr      population_ptr();
	ConstPopulationPtr population_ptr() const;

	size_t size() const;
	ublas::vector<SpatialTypes::coord_type> positions() const;

	const std::string & label() const;
	void setLabel(const std::string & label);

	bool operator==(const PopulationView& right) const;
	bool operator!=(const PopulationView& right) const;

private:
	friend std::ostream & operator<<(std::ostream & out, const PopulationView &);
	friend class boost::serialization::access;

	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int& /*version*/);

	PopulationPtr const mPopulation;
	boost::shared_ptr<mask_type> const mMask;
	size_t const mSize;
	std::string mLabel;
};

template<typename Archiver>
inline void PopulationView::serialize(Archiver& ar, const unsigned int& )
{
	using boost::serialization::make_nvp;

	ar & make_nvp("pop",  const_cast<PopulationPtr&>(mPopulation));
	ar & make_nvp("mask", const_cast<boost::shared_ptr<mask_type>&>(mMask));
	ar & make_nvp("size", const_cast<size_t&>(mSize));
	ar & make_nvp("label", mLabel);
}
