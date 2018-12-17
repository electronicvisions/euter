#include "euter/objectstore.h"

#include <algorithm>
#include <iostream>
#include <numeric>

#include <boost/variant/get.hpp>

#ifdef __linux__
#include <log4cxx/logger.h>
#else
namespace log4cxx {
typedef std::string LoggerPtr;
namespace Logger {
typedef std::string getLogger;
} // namespace log4cxx
} // namespace Logger

template <typename... Args>
void LOG4CXX_INFO(std::string prefix, Args&&... args)
{
	((std::cout << prefix) << ... << args);
}
#endif

namespace euter {

namespace {
template<typename Container>
void _print(std::ostream& os, Container const& c)
{
	size_t ii = 0;
	for (auto const& p : c)
	{
		os << "\t\t" << ii << "th element:\t" << p << "\t" << *p << std::endl;
		++ii;
	}
}
}

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("euter"));

ObjectStore::ObjectStore() :
	mPopulations(new population_vector()),
	mProjections(new projection_vector()),
	mCurrentSources(new current_sources_vector()),
	mPopulationId(0),
	mProjectionId(0),
	mNumCells(0),
	mDuration(0)
{}

double ObjectStore::getTimestep() const
{
    return mSettings.timestep;
}

double ObjectStore::getMinDelay() const
{
    return mSettings.min_delay;
}

double ObjectStore::getMaxDelay() const
{
    return mSettings.max_delay;
}

void ObjectStore::insert(PopulationPtr const& object)
{
	mPopulations->push_back(object);
}

void ObjectStore::insert(CurrentSourcePtr const& object)
{
	mCurrentSources->push_back(object);
}

void ObjectStore::insert(ProjectionPtr const& object)
{
	mProjections->push_back(object);
}

size_t ObjectStore::getCellIds(size_t N)
{
	size_t const ret = mNumCells;
	mNumCells += N;
	return ret;
}

size_t ObjectStore::nextPopulationId()
{
	return mPopulationId++;
}

size_t ObjectStore::nextProjectionId()
{
	return mProjectionId++;
}

size_t ObjectStore::numCells() const
{
	return mNumCells;
}

double ObjectStore::getDuration() const
{
	return mDuration;
}

std::ostream & operator<< (std::ostream& o, const ObjectStore & store)
{
	o << "ObjectStore:" << std::endl;
	o << "\tPopulations:" << std::endl;
	_print(o, store.populations());
	o << "\tProjections:" << std::endl;
	_print(o, store.projections());
	return o;
}

void ObjectStore::setup(const Settings & settings,
                        const metadata_map & metadata)
{
	mSettings = settings;
	mMetaData = metadata;

	std::string metadata_packages;
	for (auto m : mMetaData )
	{
		if(!metadata_packages.empty())
			metadata_packages += ", ";
		metadata_packages += m.first;
	}

	LOG4CXX_INFO(
	    logger, "ObjectStore initialized. Additional metadata: " + metadata_packages + ".");
}

void ObjectStore::reset()
{
	mDuration = 0;

	for (auto p : populations()) {
		p->reset();
	}

	for (auto p : projections()) {
		p->reset();
	}

}

void ObjectStore::run(double duration)
{
	mDuration = duration;
}

size_t ObjectStore::size() const
{
	return mPopulations->size()
		+ mProjections->size()
		+ mCurrentSources->size();
}

ObjectStore::population_vector const& ObjectStore::populations() const
{
	return *mPopulations;
}

ObjectStore::projection_vector const& ObjectStore::projections() const
{
	return *mProjections;
}

ObjectStore::current_sources_vector const&
ObjectStore::current_sources() const
{
	return *mCurrentSources;
}

ObjectStore::metadata_t& ObjectStore::getMetaData(std::string const& name)
{
	return mMetaData.at(name);
}

ObjectStore::metadata_t const& ObjectStore::getMetaData(std::string const& name) const
{
	return mMetaData.at(name);
}

void ObjectStore::setMetaData(std::string const& name, metadata_t const& md)
{
	mMetaData[name] = md;
}

ObjectStore::metadata_map const& ObjectStore::getMetaData() const
{
	return mMetaData;
}

std::vector<std::string> const& ObjectStore::argv() const
{
	return mArgv;
}

void ObjectStore::check()
{
	size_t const neurons = std::accumulate(
		mPopulations->begin(), mPopulations->end(), 0,
		[](size_t ii, PopulationPtr const& p) {
	        return ii + p->size();
		});

	if (neurons != numCells())
		throw std::runtime_error("Inconsistent state");

	for (size_t ii=1; ii<mPopulations->size(); ++ii) {
		if ((*mPopulations)[ii-1]->id() >= (*mPopulations)[ii]->id())
			throw std::runtime_error("population vector is in unordered state");
	}
}


std::string MetaDataToString::operator()(MetaDataPtr metadata) const
{
	return metadata->toString();
}

std::string MetaDataToString::operator()(std::string const& metadata) const
{
	return metadata;
}

} // namespace euter
