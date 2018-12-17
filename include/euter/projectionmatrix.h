#pragma once

#include "connector.h"

namespace euter {

class RandomGenerator;

// mixing for book keeping of weights/delays in case of non-file-dependent
// connectors
class ProjectionMatrix
{
public:
	typedef Connector::value_type  value_type;
	typedef Connector::vector_type vector_type;
	typedef Connector::matrix_type matrix_type;

	ProjectionMatrix();

	void initStructure(Assembly & pre, Assembly & post,
	                   const Connector & c,
	                   RandomGenerator & rnd_dist);

	void initDefaultValues(const Connector::default_type & c);

	/// returns the no of connections realized by the Matrix (non NaN elements)
	size_t elements() const;

	void set(value_type data);
	template <typename T, typename U>
	void set(const boost::numeric::ublas::vector<T, U> & data);
	template <class T, class L, class A>
	void set(const boost::numeric::ublas::matrix<T, L, A> & in);
    void set(boost::shared_ptr<RandomDistribution> dist);

	matrix_type const& get() const;
	matrix_type&       get();

private:
	matrix_type mMatrix;
	size_t mSize;

	void throwSizeError(size_t size1);
	void throwSizeError(size_t size1, size_t size2);

	class InitDefaultMatrixVisitor;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int);
	friend class boost::serialization::access;
};

template<class Archive>
void ProjectionMatrix::serialize(Archive & ar, const unsigned int)
{
	ar & BOOST_SERIALIZATION_NVP(mSize)
	   & BOOST_SERIALIZATION_NVP(mMatrix);
}

template <typename T, typename U>
void ProjectionMatrix::set(const boost::numeric::ublas::vector<T, U> & in)
{
    auto i_in = in.begin();
    auto i_out = mMatrix.data().begin();

    // TODO: Warning for strict mode!
    if(static_cast<size_t>(std::distance(i_in, in.end())) < mSize)
    {
        throwSizeError(std::distance(i_in, in.end()));
    }

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

template <class T, class L, class A>
void ProjectionMatrix::set(const boost::numeric::ublas::matrix<T, L, A> & in)
{
	auto i_in = in.data().begin();
	auto i_end = in.data().end();
	auto i_out = mMatrix.data().begin();

	if(in.size1() != mMatrix.size1() && in.size2() != mMatrix.size2())
	{
		throwSizeError(in.size1(), in.size2());
	}

	if(static_cast<size_t>(std::distance(i_in, i_end)) != mSize)
	{
		throwSizeError(in.size1(), in.size2());
	}

	while(i_in != i_end)
	{
		if(!std::isnan(*i_out))
		{
			assert(i_out <= mMatrix.data().end());
			*i_out = *i_in;
		}
		++i_in;
		++i_out;
	}
}

} // namespace euter
