#pragma once

#include <boost/make_shared.hpp>

#include "connector.h"
#include "space.h"

struct ProbabilityGenerator
{
	virtual ConnectorTypes::value_type operator()(SpatialTypes::distance_type distance) const = 0;
	virtual ConnectorTypes::matrix_type operator()(
	    ublas::matrix<SpatialTypes::distance_type> const& distances) const = 0;
};

class DistanceDependentProbabilityConnector : public Connector
{
public:
    DistanceDependentProbabilityConnector(
        boost::shared_ptr<ProbabilityGenerator> const& probability_generator,
        const bool            allow_self_connections = true,
        const default_type&   weights = 0.0,
        const default_type&   delays  = 0.0,
		const boost::shared_ptr<Space>& space = boost::make_shared<Space>(),
		const int n_connections = -1
		);

    virtual void printOn(std::ostream& o) const;

    virtual size_t connect(
        const Assembly & pre,
        const Assembly & post,
        RandomGenerator & rnd,
        matrix_type & matrix) const;
private:
    // TODO: Initialize callback with a stub generator
	// for serialization
    DistanceDependentProbabilityConnector();

    friend class boost::serialization::access;
    template<typename Archiver>
    void serialize(Archiver& ar, const unsigned int&) {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Connector)
           & BOOST_SERIALIZATION_NVP(mAllowSelfConnections);
    }

    bool mAllowSelfConnections;
	boost::shared_ptr<ProbabilityGenerator> mProbabilityGenerator;
	boost::shared_ptr<Space> mSpace;
	int mNConnections;
};

BOOST_CLASS_EXPORT_KEY(DistanceDependentProbabilityConnector)
