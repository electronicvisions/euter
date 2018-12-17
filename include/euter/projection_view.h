#pragma once

#include "connector.h"
#include "population_view.h"

namespace euter {

class Projection;

class ProjectionView
{
public:
	typedef PopulationView                        population_type;
	typedef boost::shared_ptr<Projection const>   projection_type;
	typedef typename boost::numeric::ublas::range range_type;
	typedef size_t                                size_type;

	ProjectionView(
		population_type const pre,
		population_type const post,
		projection_type proj,
		range_type source,
		range_type target);
	~ProjectionView();
	ProjectionView& operator=(ProjectionView const& other);
	ProjectionView() = default;
	ProjectionView(const ProjectionView&) = default;

	population_type const& pre()  const;
	population_type const& post() const;

	Connector::const_matrix_view_type getWeights() const;
	Connector::const_matrix_view_type getDelays()  const;

	size_type size() const;

	projection_type projection() const;

	std::ostream& operator<<(std::ostream& o) const;

private:
	population_type const  mPre;
	population_type const  mPost;

	mutable size_type      mSize;
	projection_type        mProjection;

	range_type             mSourceRange;
	range_type             mTargetRange;

	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int);
};

template<typename Archiver>
void ProjectionView::serialize(Archiver& ar, const unsigned int)
{
	using boost::serialization::make_nvp;

	ar & make_nvp("mPre",  const_cast<population_type&>(mPre))
	   & make_nvp("mPost", const_cast<population_type&>(mPost))
	   & BOOST_SERIALIZATION_NVP(mProjection)
	   & BOOST_SERIALIZATION_NVP(mSourceRange)
	   & BOOST_SERIALIZATION_NVP(mTargetRange);
}

} // namespace euter
