#pragma once

#include "boost/serialization/dynamic_bitset.h"
#include "euter/typedcellparametervector.h"
#include "pyparameteraccess.h"

struct GetParameterProxy
{
	typedef ParameterProxy return_type;

	template <typename TypedCellParameterVector>
	return_type operator()(TypedCellParameterVector & v, size_t index) const
	{
		return getParameterProxy(v.parameters()[index]);
	}
};

struct GetParameterProxyVector
{
	typedef std::vector<ParameterProxy> return_type;

	template <typename TypedCellParameterVector>
	return_type operator()(TypedCellParameterVector & v) const
	{
		return_type result(v.parameters().size());
		auto out = result.begin();
		auto iter = v.parameters().begin(), iend = v.parameters().end();
		for(; iter != iend; ++iter, ++out)
			*out = getParameterProxy(*iter);
		return result;
	}

	template <typename TypedCellParameterVector>
	return_type operator()(TypedCellParameterVector & v,
	                       const boost::dynamic_bitset<> & mask) const
	{
		return_type result;
		assert(v.size() == mask.size());

		auto iter = v.parameters().begin();
		for(size_t ii = 0; ii < mask.size(); ++ii)
		{
			if(mask[ii])
			{
				result.push_back(getParameterProxy(*iter));
			}
			++iter;
		}
		assert(iter <= v.parameters().end());
		assert(mask.count() == result.size());

		return result;
	}
};

