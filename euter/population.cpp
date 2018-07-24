#include "population.h"

#include <stdexcept>
#include <boost/make_shared.hpp>

#include "space.h"
#include "objectstore.h"

Population::Population(
	ObjectStore& store,
	size_t const size,
	CellType const type,
	boost::shared_ptr<Structure> const& structure,
	std::string const& label) :
		mPopulationId(store.nextPopulationId()),
		mFirstCellId(store.getCellIds(size)),
		mLabel(label),
		mStructure(structure), // is set below
		mPositions(),
		mCellparams(CellParameterVector::create(size, type)),
		mSpikes(size),
		mMembraneVoltages(size)
{
	if(size == 0) {
		throw std::runtime_error("Empty populations are not allowed");
	}
}

PopulationPtr Population::create(
	ObjectStore& store,
	size_t size,
	CellType const type)
{
	return create(store, size, type, boost::make_shared<Line>(), "");
}

PopulationPtr Population::create(
	ObjectStore& store,
	size_t size,
	CellType const type,
	boost::shared_ptr<Structure> const& structure,
	std::string const& label)
{
	boost::shared_ptr<Population> obj(new Population(store, size, type, structure, label));
	store.insert(obj);
	return obj;
}

size_t Population::size() const
{
	return mCellparams->size();
}

size_t Population::id() const
{
	return mPopulationId;
}

CellType Population::type() const
{
	return mCellparams->type();
}

std::string const& Population::label() const
{
	return mLabel;
}

CellParameterVector & Population::parameters()
{
	return *mCellparams;
}

const CellParameterVector & Population::parameters() const
{
	return *mCellparams;
}

size_t Population::globalNeuronId(size_t const n) const
{
	if (n >= size())
		throw std::out_of_range("Invalid index");
	return mFirstCellId + n;
}

size_t Population::firstNeuronId() const
{
	return mFirstCellId;
}

std::ostream& operator<< (std::ostream& o, const Population & p)
{
	o << "Population(id=" << p.id() << ", type=" << getCellTypeName(p.type())
	  << ", size=" << p.size() << ", label=\"" << p.label() << "\")";
	return o;
}

// Should we return and set references or copies?
ublas::vector<SpatialTypes::coord_type> Population::positions() const
{
	if (mPositions.empty()) {
		// FIXME: this is not internally synchronized
		const_cast<ublas::vector<SpatialTypes::coord_type>&>(mPositions)
			= mStructure->generatePositions(size());
	}
	return mPositions;
}

void Population::setPositions(ublas::vector<SpatialTypes::coord_type> const& positions)
{
	if (positions.size() != size())
		throw std::runtime_error("missmatch in position dimensions");
	mPositions = positions;
}

boost::shared_ptr<Structure const> Population::getStructure() const
{
	return mStructure;
}

boost::shared_ptr<Structure> Population::getStructure()
{
	return mStructure;
}

void Population::setStructure(const boost::shared_ptr<Structure>& structure)
{
	// Reset positions in order to regenerate them when used.
	mStructure = structure;
}

std::vector<float>& Population::getSpikes(size_t const n)
{
	return mSpikes.at(n);
}

std::vector<float> const& Population::getSpikes(size_t const n) const
{
	return mSpikes.at(n);
}

std::vector<std::pair<float, float> >& Population::getMembraneVoltageTrace(size_t const n)
{
	return mMembraneVoltages.at(n);
}

std::vector<std::pair<float, float> > const& Population::getMembraneVoltageTrace(size_t const n) const
{
	return mMembraneVoltages.at(n);
}


bool Population::operator==(const Population& right)
{
	return (this->id() == right.id());
}

void Population::reset() {
	for (auto& s : mSpikes) {
		s.clear();
	}

	for(auto& m : mMembraneVoltages) {
		m.clear();
	}

	// TODO: clear other recorded data
}
