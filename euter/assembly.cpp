#include "assembly.h"

#include "objectstore.h"

Assembly::Assembly(std::string const& label) :
	mLabel(label)
{
	updateSize();
}

Assembly::Assembly(boost::shared_ptr<Population> population, std::string const& label) :
	mLabel(label),
	mMembers({PopulationView(population)})
{
	updateSize();
}

Assembly::Assembly(PopulationView view, std::string const& label) :
	mLabel(label),
	mMembers({view})
{
	updateSize();
}

Assembly::Assembly(list_type const& members, std::string const& label) :
	mLabel(label),
	mMembers(members)
{
	updateSize();
}

size_t Assembly::size() const
{
	return mSize;
}

std::string const& Assembly::label() const
{
	return mLabel;
}

Assembly::list_type::iterator Assembly::begin()
{
	return mMembers.begin();
}

Assembly::list_type::const_iterator Assembly::begin() const
{
	return mMembers.begin();
}

Assembly::list_type::const_iterator Assembly::cbegin() const
{
	return mMembers.begin();
}

Assembly::list_type::iterator Assembly::end()
{
	return mMembers.end();
}

Assembly::list_type::const_iterator Assembly::end() const
{
	return mMembers.end();
}

Assembly::list_type::const_iterator Assembly::cend() const
{
	return mMembers.end();
}

void Assembly::append(Assembly const& assembly)
{
	append(assembly.mMembers);
}

void Assembly::append(list_type const& members)
{
	mMembers.reserve(mMembers.size() + members.size());
	for (auto const& view : members) {
		mMembers.push_back(view);
	}
	updateSize();
}

Assembly::list_type& Assembly::populations()
{
	return this->mMembers;
}

const Assembly::list_type& Assembly::populations() const
{
	return this->mMembers;
}

ublas::vector<SpatialTypes::coord_type> Assembly::positions() const
{
    ublas::vector<SpatialTypes::coord_type> positions(size());

    auto j=positions.begin();

	for (auto const& i : mMembers) {
        for (auto const& point : i.positions()) {
            *j++ = point;
        }
    }

    return positions;
}

void Assembly::updateSize()
{
	mSize = 0;
	for (auto p : mMembers)
	{
		mSize += p.size();
	}
}

bool Assembly::operator==(const Assembly& right) const
{
	if(this->size() != right.size()) return false;
	if(mMembers.size() != right.mMembers.size()) return false;

	for(size_t i = 0; i < mMembers.size(); ++i)
	{
		if(mMembers[i] != right.mMembers[i]) return false;
	}
	return true;
}

std::ostream& operator<< (std::ostream & o, const Assembly & a)
{
	o << "PyAssembly (" << a.size() << ")" ;
	return o;
}
