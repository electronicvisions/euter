#include "euter/projection_view.h"
#include "euter/projection.h"

namespace euter {

ProjectionView::ProjectionView(
	population_type const pre,
	population_type const post,
	projection_type proj,
	range_type source,
	range_type target) :
		mPre(pre),
		mPost(post),
		mSize(std::numeric_limits<size_t>::max()),
		mProjection(proj),
		mSourceRange(source),
		mTargetRange(target)
{}

ProjectionView& ProjectionView::operator=(ProjectionView const& other)
{
	if (&other != this)
	{
		new (this) ProjectionView(other);
	}
	return *this;
}

ProjectionView::~ProjectionView() {}

ProjectionView::population_type const&
ProjectionView::pre() const
{
	return mPre;
}

ProjectionView::population_type const&
ProjectionView::post() const
{
	return mPost;
}

Connector::const_matrix_view_type ProjectionView::getWeights() const
{
	return Connector::const_matrix_view_type(
	            mProjection->getWeights().get(),
	            mSourceRange,
	            mTargetRange);
}

Connector::const_matrix_view_type ProjectionView::getDelays()  const
{
	return Connector::const_matrix_view_type(
	            mProjection->getDelays().get(),
	            mSourceRange,
	            mTargetRange);
}

ProjectionView::size_type ProjectionView::size() const
{
	if (mSize == std::numeric_limits<size_t>::max()) {

		mSize = 0;
		Connector::const_matrix_view_type weights = getWeights();

		for (size_t src = 0; src < weights.size1(); ++src) {
			for (size_t trg = 0; trg < weights.size2(); ++trg) {
				if (!std::isnan(weights(src, trg)))
					++mSize;
			}
		}
	}
	return mSize;
}

ProjectionView::projection_type ProjectionView::projection() const
{
	return mProjection;
}

std::ostream& ProjectionView::operator<<(std::ostream& o) const
{
	// FIXME
	//o << "ProjectionView (";
	//mProjection->printOn(o);
	//o << ")";
	return o;
}

} // namespace euter
