#include "euter/projectionmatrix.h"

#include <algorithm>
#include <sstream>

#include "euter/connector.h"
#include "euter/assembly.h"
#include "euter/exceptions.h"
#include "euter/random.h"


class ProjectionMatrix::InitDefaultMatrixVisitor : public boost::static_visitor<void>
{
public:
	InitDefaultMatrixVisitor(Connector::matrix_type & matrix,
	                         ProjectionMatrix & obj) :
	mMatrix(matrix), mObj(obj) {}

	void operator()(Connector::value_type v) const
	{
		if (v != 0.0)
		{
			mObj.set(v);
		}
	}

	void operator()(const Connector::vector_type & v) const
	{
		mObj.set(v);
	}

	void operator()(const Connector::matrix_type & v) const
	{
		mObj.set(v);
	}

    void operator()(const boost::shared_ptr<RandomDistribution> v) const
    {
        mObj.set(v);
    }

	template <typename T>
	void operator()(T) const
	{
		NOT_IMPLEMENTED();
	}

private:
	Connector::matrix_type & mMatrix;
	ProjectionMatrix & mObj;
};


ProjectionMatrix::ProjectionMatrix() :
    mSize(0)
{}

void ProjectionMatrix::initStructure(Assembly & pre,
                                     Assembly & post,
                                     const Connector & c,
                                     RandomGenerator & rnd_dist)
{
	mSize = c.initMatrix(pre, post, mMatrix, rnd_dist);

	boost::apply_visitor(InitDefaultMatrixVisitor(mMatrix, *this),
	                     c.getDefaultDelays());
	assert(mMatrix.size1() * mMatrix.size2() == pre.size() * post.size());
	assert(mSize <= mMatrix.size1() * mMatrix.size2());
}

void ProjectionMatrix::initDefaultValues(const Connector::default_type & value)
{
	boost::apply_visitor(InitDefaultMatrixVisitor(mMatrix, *this), value);
}

size_t ProjectionMatrix::elements() const
{
	return mSize;
}

ProjectionMatrix::matrix_type const& ProjectionMatrix::get() const
{
	return mMatrix;
}

ProjectionMatrix::matrix_type& ProjectionMatrix::get()
{
	return mMatrix;
}

void ProjectionMatrix::set(boost::shared_ptr<RandomDistribution> dist)
{
    std::vector<distribution_float_t> tmp(elements());
    dist->next(tmp);
    
    auto i_out = mMatrix.data().begin();
    auto i_in = tmp.begin();

    while(i_out < mMatrix.data().end())
    {
        if(!std::isnan(*i_out))
        {
            assert(i_out <= mMatrix.data().end());
            *i_out = *i_in;
            ++i_in;
        }
        ++i_out;
    }

}

void ProjectionMatrix::set(value_type in)
{
	auto it = mMatrix.data().begin();
	auto iend = mMatrix.data().end();
	for(; it != iend ;++it)
	{
		if(!std::isnan(*it))
		{
			*it = in;
		}
	}
}

void ProjectionMatrix::throwSizeError(size_t size)
{
    std::stringstream message;
    message << "Invalid input vector size: expected " << elements()
            << ", got " << size;
    throw InvalidDimensions(message.str());
}

void ProjectionMatrix::throwSizeError(size_t size1, size_t size2)
{
    std::stringstream message;
    message << "Invalid input matrix size: expected "
            << "(" << mMatrix.size1() << ", " << mMatrix.size2() << ")"
            << ", got (" << size1 << ", " << size2 << ")";
    throw InvalidDimensions(message.str());
}
