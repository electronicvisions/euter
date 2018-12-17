#include "euter/population_view.h"

#include "euter/population.h"
#include "euter/typedcellparametervector.h"

namespace euter {

PopulationView::PopulationView() :
    mSize(0)
{
}

PopulationView::PopulationView(PopulationPtr population) :
    mPopulation(population),
    mMask(new mask_type(population->size())),
    mSize(population ? population->size() : 0),
	mLabel(mPopulation->label())
{
	mMask->set();
}

PopulationView::PopulationView(
		PopulationPtr population,
		mask_type const& mask) :
	mPopulation(population),
	mMask(new mask_type(mask)),
	mSize(mMask->count()),
	mLabel("view of \"" + mPopulation->label() + "\" containing " + std::to_string(mSize))
{
}

PopulationView & PopulationView::operator=(const PopulationView & other)
{
	if (&other != this)
	{
		new (this) PopulationView(other);
	}
	return *this;
}

PopulationView PopulationView::copy_with_mask(mask_type const& mask) const
{
	return PopulationView(this->mPopulation, mask);
}

Population const& PopulationView::population() const
{
	return *mPopulation;
}

Population& PopulationView::population()
{
	return *mPopulation;
}

PopulationPtr PopulationView::population_ptr()
{
	return mPopulation;
}

ConstPopulationPtr PopulationView::population_ptr() const
{
	return mPopulation;
}

PopulationView::mask_type const& PopulationView::mask() const
{
	return *mMask;
}

size_t PopulationView::size() const
{
	return mSize;
}

ublas::vector<SpatialTypes::coord_type> PopulationView::positions() const
{
    ublas::vector<SpatialTypes::coord_type> positions(size());

    size_t next_neuron = mask().find_first();
    for(size_t ii=0; ii<size(); ii++)
    {
        positions(ii) = population().positions()[next_neuron];
        next_neuron = mask().find_next(next_neuron);
    }
    return positions;
}

const std::string & PopulationView::label() const
{
	return mLabel;
}

void PopulationView::setLabel(const std::string & label)
{
	mLabel = label;
}

std::ostream & operator<<(std::ostream & out, const PopulationView & v)
{
	out << "PopulationView (" << v.size() << " of " << v.population() << ")";
	return out;
}

bool PopulationView::operator==(const PopulationView& right) const
{
	return (this->population_ptr() == right.population_ptr() && this->mask() == right.mask());
}

bool PopulationView::operator!=(const PopulationView& right) const
{
	return !(*this == right);
}

} // namespace euter
