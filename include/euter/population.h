#pragma once

#include <vector>
#include <memory>
#include <string>

#include "cellparametervector.h"
#include "typedcellparametervector.h"
#include "space.h"

#include <boost/numeric/ublas/matrix.hpp>
#include "boost/serialization/serialization.h"
#include "boost/serialization/ublas.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

namespace euter {

class ObjectStore;
class Population;
typedef boost::shared_ptr<Population> PopulationPtr;
typedef boost::shared_ptr<Population const> ConstPopulationPtr;

// The C++ Impelentation of an PyPopulation Object
class Population
{
public:
	// Creates an instance of the type Population and registeres it with the
	// CommandStore
	static PopulationPtr create(
		ObjectStore& store,
		size_t size,
		CellType type);

	static PopulationPtr create(
		ObjectStore& store,
		size_t size,
		CellType type,
		boost::shared_ptr<Structure> const& structure,
		std::string const& label);

	/// @ret size of the population
	size_t size() const;
	size_t id() const;
	CellType type() const;
	std::string const& label() const;

	/// @ret the unique id of the n-th neuron in this population
	size_t globalNeuronId(size_t n = 0) const;
	size_t firstNeuronId() const;

	CellParameterVector & parameters();
	const CellParameterVector & parameters() const;

	ublas::vector<SpatialTypes::coord_type> positions() const;
	void setPositions(ublas::vector<SpatialTypes::coord_type> const& positions);

	boost::shared_ptr<Structure>       getStructure();
	boost::shared_ptr<Structure const> getStructure() const;
	void setStructure(const boost::shared_ptr<Structure>& structure);

	std::vector<float>&       getSpikes(size_t n);
	std::vector<float> const& getSpikes(size_t n) const;
	std::vector<std::pair<float/*time*/, float/*value*/> >&       getMembraneVoltageTrace(size_t n);
	std::vector<std::pair<float/*time*/, float/*value*/> > const& getMembraneVoltageTrace(size_t n) const;

	/// clear spikes (and other recorded data once supported)
	void reset();

private:
	// default constructor ONLY for serialization
	Population() = default;
	Population(Population&&) = delete;
	Population(Population const&) = delete;
	Population(ObjectStore& store,
			size_t size,
			CellType type,
			boost::shared_ptr<Structure> const& structure,
			std::string const& label = "");

	friend class boost::serialization::access;

	template<typename Archiver>
	void serialize(Archiver& ar, unsigned int version);

	size_t mPopulationId;
	size_t mFirstCellId;
	std::string mLabel;
	boost::shared_ptr<Structure> mStructure;
	ublas::vector<SpatialTypes::coord_type> mPositions;
	std::unique_ptr<CellParameterVector> mCellparams;

	// lower software layers store time in seconds
	std::vector<std::vector<float>> mSpikes;
	std::vector<std::vector<std::pair<float, float>>> mMembraneVoltages;

	bool operator==(const Population& right);
};

std::ostream& operator<< (std::ostream& o, const Population & store);

template<typename Archiver>
void Population::serialize(Archiver& ar, unsigned int const /*version*/)
{
	RegisterCellTypeForSerializationVisitor<Archiver, TypedCellParameterVector> v(ar);
	forallCellTypes(v);

	ar & BOOST_SERIALIZATION_NVP(mPopulationId)
	   & BOOST_SERIALIZATION_NVP(mFirstCellId)
	   & BOOST_SERIALIZATION_NVP(mLabel)
	   // FIXME: add serializers to structure classes
	   //& BOOST_SERIALIZATION_NVP(mStructure)
	   & BOOST_SERIALIZATION_NVP(mPositions)
	   & BOOST_SERIALIZATION_NVP(mCellparams)
	   & BOOST_SERIALIZATION_NVP(mSpikes)
	   & BOOST_SERIALIZATION_NVP(mMembraneVoltages);
}

} // namespace euter
