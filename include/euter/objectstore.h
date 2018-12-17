#pragma once

#include <set>
#include <iosfwd>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include "boost/serialization/serialization.h"
#include "boost/serialization/shared_ptr.hpp"
#include "population.h"
#include "current.h"
#include "projection.h"
#include "parameter.h"
#include "metadata.h"

namespace euter {

/// This singleton keeps track of pynn objects created. It allows serialization
/// to send pynn objects
/// @note currently not threadsafe
class ObjectStore
{
public:
	typedef std::vector<PopulationPtr> population_vector;
	typedef std::vector<CurrentSourcePtr> current_sources_vector;
	typedef std::vector<ProjectionPtr> projection_vector;

	typedef boost::variant<MetaDataPtr, std::string> metadata_t;
	typedef std::map<std::string, metadata_t> metadata_map;

	ObjectStore();

	PARAMETER_STRUCT(Settings,
	    PARAMETER(double, timestep, 0.1),
	    PARAMETER(double, min_delay, 0.1),
	    PARAMETER(double, max_delay, 10.0))

    double getTimestep() const;
    double getMinDelay() const;
    double getMaxDelay() const;

	// euter interface

	/// insert PyNN commands into ObjectStore (they shoud not be public
	/// interface, but only available for Pops, Projs, and Currents)
	void insert(PopulationPtr const& object);
	void insert(CurrentSourcePtr const& object);
	void insert(ProjectionPtr const& object);

	/// auxilary commands for insertions above (they shoud not be public
	/// interface, but only available for Pops, Projs, and Currents)
	size_t getCellIds(size_t N);
	size_t nextPopulationId();
	size_t nextProjectionId();

	/// total number of PyNN Commands stored in this ObjectStore
	/// (Populations+Projections+CurrentSources).
	size_t size() const;
	/// number neurons in total populations
	size_t numCells() const;
	/// experiment duration in ms
	double getDuration() const;


	// PyNN Interface
	void setup(const Settings & settings,
	           const metadata_map & metadata = metadata_map());
	void run(double duration /*ms*/);
	void reset();

	population_vector const& populations() const;
	projection_vector const& projections() const;
	current_sources_vector const& current_sources() const;


	/// access to users argv
	std::vector<std::string> const& argv() const;

	// MetaData interface
	template<typename DerivedMetaData>
	boost::shared_ptr<DerivedMetaData const> getMetaData(std::string const& name) const;
	template<typename DerivedMetaData>
	boost::shared_ptr<DerivedMetaData> getMetaData(std::string const& name);

	metadata_t& getMetaData(std::string const& name);
	metadata_t const& getMetaData(std::string const& name) const;
	void setMetaData(std::string const& name, metadata_t const& md);

	metadata_map const& getMetaData() const;

	/// check ObjectStore for consistency
	void check();

private:
	friend std::ostream& operator<<(std::ostream& o, ObjectStore const& store);
	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int&);

	/// list of associated populations
	boost::shared_ptr<population_vector> mPopulations;

	/// list of associated projections
	boost::shared_ptr<projection_vector> mProjections;

	/// list of current stimuli
	boost::shared_ptr<current_sources_vector> mCurrentSources;

	/// next available population id
	size_t mPopulationId;

	/// next available projection id
	size_t mProjectionId;

	/// number of neurons in this ObjectStore
	size_t mNumCells;

	/// experiment duration in ms
	double mDuration;

	Settings mSettings;
	metadata_map mMetaData;
	std::vector<std::string> mArgv;
};


class MetaDataToString :
	public boost::static_visitor<std::string>
{
public:
	std::string operator()(MetaDataPtr metadata) const;
	std::string operator()(const std::string & metadata) const;
};

template <typename T>
class MetaDataFromString :
	public boost::static_visitor< boost::shared_ptr<T> >
{
public:
	boost::shared_ptr<T> operator()(MetaDataPtr metadata) const;
	boost::shared_ptr<T> operator()(std::string const& metadata) const;
};

template<typename Archiver>
void ObjectStore::serialize(Archiver& ar, const unsigned int&)
{
	ar & BOOST_SERIALIZATION_NVP(mPopulations)
	   & BOOST_SERIALIZATION_NVP(mProjections)
	   & BOOST_SERIALIZATION_NVP(mCurrentSources)
       & BOOST_SERIALIZATION_NVP(mPopulationId)
       & BOOST_SERIALIZATION_NVP(mNumCells)
       & BOOST_SERIALIZATION_NVP(mDuration)
	   & BOOST_SERIALIZATION_NVP(mSettings)
	   & BOOST_SERIALIZATION_NVP(mArgv);

	// Metadata is stored via strings (internally also boost archives), because
	// we don't know possible metadata implentetations here
	if (Archiver::is_saving::value)
	{
		std::map<std::string, std::string> metadata;
		for (auto it = mMetaData.begin(); it != mMetaData.end(); ++it)
		{
			metadata[it->first] = boost::apply_visitor(MetaDataToString(),
			                                           it->second);
		}
		ar & BOOST_SERIALIZATION_NVP(metadata);
	}

	if (Archiver::is_loading::value)
	{
		std::map<std::string, std::string> metadata;
		ar & BOOST_SERIALIZATION_NVP(metadata);
		for (auto it = metadata.begin(); it != metadata.end(); ++it)
		{
			mMetaData[it->first] = it->second;
		}
	}
}

template<typename DerivedMetaData>
boost::shared_ptr<DerivedMetaData const>
ObjectStore::getMetaData(const std::string & name) const
{
	auto it = mMetaData.find(name);
	if (it != mMetaData.end())
	{
		boost::shared_ptr<DerivedMetaData> m = boost::apply_visitor(
		            MetaDataFromString<DerivedMetaData>(), it->second);
		if (m.get() == nullptr)
		{
			throw std::runtime_error("Invalid MetaData entry for " + name);
		}

		// Ensure that we now keep the metadata pointer available
		const_cast<ObjectStore*>(this)->mMetaData[name] = m;
		return m;
	}
	return DerivedMetaData::create();
}

template<typename DerivedMetaData>
boost::shared_ptr<DerivedMetaData>
ObjectStore::getMetaData(const std::string & name)
{
	ObjectStore const& _this = *this;
	return boost::const_pointer_cast<DerivedMetaData>(
		_this.getMetaData<DerivedMetaData>(name));
}

template <typename T>
boost::shared_ptr<T> MetaDataFromString<T>::operator()(MetaDataPtr metadata) const
{
	return boost::dynamic_pointer_cast<T>(metadata);
}

template <typename T>
boost::shared_ptr<T> MetaDataFromString<T>::operator()(std::string const& metadata) const
{
	boost::shared_ptr<T> ret = T::create();
	ret->fromString(metadata);
	return ret;
}

} // namespace euter
