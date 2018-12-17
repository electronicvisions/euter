#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "boost/serialization/serialization.h"
#include "boost/serialization/ublas.hpp"
#include "random.h"

namespace euter {

namespace ublas = boost::numeric::ublas;

struct ConnectorTypes
{
	typedef float                                        value_type;
	typedef ublas::vector<value_type>                    vector_type;

	typedef ublas::matrix<value_type>                    matrix_type;
	typedef ublas::matrix_range<matrix_type>             matrix_view_type;
	typedef ublas::matrix_column<matrix_type>            matrix_col_type;
    typedef ublas::matrix_row<matrix_type>               matrix_row_type;

	typedef ublas::matrix<value_type> const              const_matrix_type;
	typedef ublas::matrix_range<const_matrix_type>       const_matrix_view_type;

	typedef ublas::matrix_column<const_matrix_type>      const_matrix_col_type;
    typedef ublas::matrix_row<const_matrix_type>         const_matrix_row_type;

	typedef boost::variant<
	         value_type,
	         vector_type,
	         matrix_type,
	         boost::shared_ptr<RandomDistribution>
	>                                                 default_type;
};

} // namespace euter
