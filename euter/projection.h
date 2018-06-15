#pragma once

#include "connector_types.h"
#include "population_view.h"
#include "random.h"
#include "assembly.h"
#include "synapses.h"

#include "projectionmatrix.h"
#include "projection_view.h"

#include <boost/enable_shared_from_this.hpp>

class ObjectStore;
class Projection;
typedef boost::shared_ptr<Projection> ProjectionPtr;
typedef boost::shared_ptr<Projection const> ConstProjectionPtr;

class Projection :
	public ConnectorTypes,
	public boost::enable_shared_from_this<Projection>
{
public:
	typedef Assembly population_type;
	typedef std::string source_type;
	typedef std::string synapse_type;

	Projection(); // FIXME: should be protected/private and mapping::GraphBuilder should be friend
	virtual ~Projection();

	static ProjectionPtr
	create(ObjectStore& store,
		   population_type presynaptic_population,
		   population_type postsynaptic_population,
		   boost::shared_ptr<Connector> method,
		   boost::shared_ptr<RandomGenerator> const& rng,
		   source_type source = "",
		   synapse_type target = "excitatory",
		   const boost::shared_ptr<SynapseDynamics>& synapse_dynamics = boost::shared_ptr<SynapseDynamics>()
		   );
		   // TODO: label
		   // TODO: rng

	const population_type & pre()  const;
	const population_type & post() const;

	source_type source() const;
	synapse_type target() const;

	size_t size() const;
	size_t id() const;

	boost::shared_ptr<SynapseDynamics> dynamics();
	boost::shared_ptr<const SynapseDynamics> dynamics() const;

	ProjectionMatrix & getDelays();
	ProjectionMatrix & getWeights();
	ProjectionMatrix const & getDelays() const;
	ProjectionMatrix const & getWeights() const;

	matrix_type const & getRawDelays() const;
	matrix_type const & getRawWeights() const;

	virtual void printOn(std::ostream& o) const;

	std::list<ProjectionView> flatten() const;

	void reset();

private:
	Projection(
			ObjectStore& store,
	        population_type presynaptic_population,
	        population_type postsynaptic_population,
	        boost::shared_ptr<Connector> method,
	        const boost::shared_ptr<RandomGenerator> & rng,
	        source_type source = "",
	        synapse_type target = "",
			const boost::shared_ptr<SynapseDynamics>& synapse_dynamics = boost::shared_ptr<SynapseDynamics>()
	        // TODO: SynapseDynamics = None
	        // TODO: label = None
	        );

	size_t mProjectionId;
	population_type mPre; // TODO: wrong type + fix serialize() below
	population_type mPost;
	boost::shared_ptr<Connector> mMethod;
	boost::shared_ptr<RandomGenerator> mRng;
	source_type mSource;
	synapse_type mTarget;
	boost::shared_ptr<SynapseDynamics> mSynapseDynamics;

	ProjectionMatrix mDelays;
	ProjectionMatrix mWeigths;

	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int);

};

std::ostream& operator<< (std::ostream& o, const Projection & proj);

template<typename Archiver>
void Projection::serialize(Archiver& ar, const unsigned int) {
	/* add other derived connector classes here due to base pointer serialization */
	ar & BOOST_SERIALIZATION_NVP(mPre)
	   & BOOST_SERIALIZATION_NVP(mPost)
	   & BOOST_SERIALIZATION_NVP(mMethod)
	   & BOOST_SERIALIZATION_NVP(mSource)
	   & BOOST_SERIALIZATION_NVP(mTarget)
	   & BOOST_SERIALIZATION_NVP(mSynapseDynamics)
	   & BOOST_SERIALIZATION_NVP(mProjectionId)
	   & BOOST_SERIALIZATION_NVP(mDelays)
	   & BOOST_SERIALIZATION_NVP(mWeigths)
	   & BOOST_SERIALIZATION_NVP(mRng);
}
