#include "euter/projection.h"

#include <boost/make_shared.hpp>

#include "euter/objectstore.h"
#include "euter/connector.h"
#include "euter/population.h"
#include "euter/objectstore.h"

#include "euter/nativerandomgenerator.h"

namespace euter {

Projection::Projection() {}

Projection::Projection(
		ObjectStore& store,
        population_type presynaptic_population,
        population_type postsynaptic_population,
        boost::shared_ptr<Connector> method,
        const boost::shared_ptr<RandomGenerator> & rng,
        source_type source,
        synapse_type target,
		const boost::shared_ptr<SynapseDynamics>& synapse_dynamics,
		label_type label
        ) :
	mProjectionId(store.nextProjectionId()),
    mPre(presynaptic_population),
    mPost(postsynaptic_population),
    mMethod(method),
    mRng(rng),
    mSource(source),
    mTarget(target),
	mSynapseDynamics(synapse_dynamics),
	mLabel(label)
{
	if (!mRng)
	{
		throw std::invalid_argument("rng is not a valid RandomNumberGenerator instance");
	}

	mWeigths.initStructure(mPre, mPost, *mMethod, *mRng);

	mDelays = mWeigths;
	mDelays.initDefaultValues(mMethod->getDefaultDelays());
	mWeigths.initDefaultValues(mMethod->getDefaultWeights());
}

Projection::~Projection() {}

ProjectionPtr Projection::create(
	ObjectStore& store,
	population_type  pre,
	population_type  post,
	boost::shared_ptr<Connector> m,
	const boost::shared_ptr<RandomGenerator> & rng,
	source_type s,
	synapse_type t,
	const boost::shared_ptr<SynapseDynamics>& synapse_dynamics,
	label_type l
	)
{
	ProjectionPtr proj(new Projection(store, pre, post, m, rng, s, t, synapse_dynamics, l));
	store.insert(proj);

	return proj;
}

size_t Projection::size() const
{
	assert(mWeigths.elements() == mDelays.elements());
	return mWeigths.elements();
}

size_t Projection::id() const
{
	return mProjectionId;
}

const Projection::population_type & Projection::pre()  const
{
	return mPre;
}

const Projection::population_type & Projection::post() const
{
	return mPost;
}

Projection::source_type Projection::source() const
{
	return mSource;
}

Projection::synapse_type Projection::target() const
{
	return mTarget;
}

Projection::label_type Projection::label() const
{
	return mLabel;
}

boost::shared_ptr<SynapseDynamics> Projection::dynamics()
{
	return mSynapseDynamics;
}

boost::shared_ptr<const SynapseDynamics> Projection::dynamics() const
{
	return mSynapseDynamics;
}

ProjectionMatrix & Projection::getWeights()
{
	return mWeigths;
}

ProjectionMatrix const & Projection::getWeights() const
{
	return mWeigths;
}

ProjectionMatrix::matrix_type const & Projection::getRawDelays() const
{
	return mDelays.get();
}

ProjectionMatrix::matrix_type const & Projection::getRawWeights() const
{
	return mWeigths.get();
}

ProjectionMatrix & Projection::getDelays()
{
	return mDelays;
}

ProjectionMatrix const & Projection::getDelays() const
{
	return mDelays;
}

void Projection::printOn(std::ostream& o) const
{
	o << "id: " << id() << ", size: " << size() << ", connector: \"";
	mMethod->printOn(o);
	o << "\", target: \"" << target() << "\", source: \"" << source() << "\", label: \"" << label()
	  << "\"";
}

std::ostream& operator<< (std::ostream& o, const Projection & p)
{

	p.printOn(o);

	return o;
}


std::list<ProjectionView> Projection::flatten() const
{
	std::list<ProjectionView> r;

	size_t pre_cnt = 0;
	for (auto const& pre_view : pre()) {
		size_t post_cnt = 0;

		for (auto const& post_view : post()) {

			r.emplace_back(
				pre_view, post_view,
				shared_from_this(),
				boost::numeric::ublas::range(pre_cnt,  pre_cnt  + pre_view.size()),
				boost::numeric::ublas::range(post_cnt, post_cnt + post_view.size())
			);

			post_cnt += post_view.size();
		}

		pre_cnt += pre_view.size();
	}
	return r;
}

void Projection::reset()
{
	// FIXME: weights should be reset to pre-STDP values, but we don't
	// store the post-STDP values -> nothing to reset

	// FIXME: reset delays although not requested in pynn API?
	// mDelays.initDefaultValues(mMethod->getDefaultDelays());
}

} // namespace euter
