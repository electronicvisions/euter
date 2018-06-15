#pragma once

#include <vector>

#include "population_view.h"

class Assembly
{
public:
	typedef std::vector<PopulationView> list_type;

	Assembly(std::string const& label = "");
	Assembly(boost::shared_ptr<Population> population, std::string const& label = "");
	Assembly(PopulationView view, std::string const& label = "");
	Assembly(list_type const& members, std::string const& label = "");

	size_t size() const;
	std::string const& label() const;

	list_type& populations();
	const list_type& populations() const;
	
	ublas::vector<SpatialTypes::coord_type> positions() const;

	list_type::iterator        begin();
	list_type::const_iterator  begin() const;
	list_type::const_iterator cbegin() const;

	list_type::iterator        end();
	list_type::const_iterator  end() const;
	list_type::const_iterator cend() const;

	// FIXME: appending is generally a stupid idea as soon as Assembly is
	// projecting onto some other assembly or projected to. Because, by changing
	// it's size we invalidate the connection matrix of any participating
	// connection. Therefore we must either break += operator & co conventions
	// and return new istances rather than extending the existing one. Or
	// disallow any resizing after the first projection has been attached.
	void append(Assembly const& assembly);
	void append(list_type const& members);

	bool operator==(const Assembly& right) const;
private:
	size_t mSize;
	std::string mLabel;
	list_type mMembers;

	void updateSize();

	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int& version);
};

std::ostream& operator<< (std::ostream & o, const Assembly & a);

template<typename Archiver>
inline void Assembly::serialize(Archiver& ar, const unsigned int& /*version*/)
{
	ar & BOOST_SERIALIZATION_NVP(mSize)
	   & BOOST_SERIALIZATION_NVP(mLabel)
	   & BOOST_SERIALIZATION_NVP(mMembers);
}
