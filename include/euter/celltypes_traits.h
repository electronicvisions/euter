#pragma once

#include <boost/type_traits.hpp>
#include "celltypes.h"
#include "cellparameters.h"

namespace euter {

// SJ: was hast du dir dabei gedacht lieber Herr Koke? Why not autogenerate this
// the mapping crom CellType to PyNNParametes via boost PP?

struct CommonCellTypeTraits
{
	static bool supported() { return false; }

	static const bool is_source = false;
};

template <CellType Type>
struct CellTypeTraits : CommonCellTypeTraits
{
	PARAMETER_STRUCT(Parameters,
	    PARAMETER(double, dummy, -1.1))

};

template<>
struct CellTypeTraits<CellType::EIF_cond_exp_isfa_ista> : CommonCellTypeTraits
{
public:
	static bool supported() { return true; }

	typedef PyNNParameters::EIF_cond_exp_isfa_ista Parameters;

	// this should in fact be std::is_trivially_copyable,
	// but not yet supported by gcc4.7.
	static_assert(boost::has_trivial_copy<Parameters>::value,
				  "Paramters should be trivially copyable");
};

template<>
struct CellTypeTraits<CellType::IF_cond_exp> : CommonCellTypeTraits
{
public:
	static bool supported() { return true; }

	typedef PyNNParameters::IF_cond_exp Parameters;

	// this should in fact be std::is_trivially_copyable,
	// but not yet supported by gcc4.7.
	static_assert(boost::has_trivial_copy<Parameters>::value,
				  "Paramters should be trivially copyable");
};

template<>
struct CellTypeTraits<CellType::EIF_multicond_exp_isfa_ista> : CommonCellTypeTraits
{
public:
	static bool supported() { return true; }

	typedef PyNNParameters::EIF_multicond_exp_isfa_ista Parameters;
};


template<>
struct CellTypeTraits<CellType::IF_multicond_exp> : CommonCellTypeTraits
{
public:
	static bool supported() { return true; }

	typedef PyNNParameters::IF_multicond_exp Parameters;
};



template<>
struct CellTypeTraits<CellType::SpikeSourceArray> : CommonCellTypeTraits
{
	static bool supported() { return true; }
	typedef PyNNParameters::SpikeSourceArray Parameters;

	static_assert(!(std::is_standard_layout<Parameters>::value &&
	                std::is_trivial<Parameters>::value), "Parameters should not be an POD");

	static const bool is_source = true;
};

template<>
struct CellTypeTraits<CellType::SpikeSourceInhGamma> : CommonCellTypeTraits
{
	static const bool is_source = true;

	typedef PyNNParameters::SpikeSourceInhGamma Parameters;

	static_assert(!(std::is_standard_layout<Parameters>::value &&
	                std::is_trivial<Parameters>::value), "Parameters should not be an POD");
};

template<>
struct CellTypeTraits<CellType::SpikeSourcePoisson> : CommonCellTypeTraits
{
	static bool supported() { return true; }
	static const bool is_source = true;

	typedef PyNNParameters::SpikeSourcePoisson Parameters;

	// this should in fact be std::is_trivially_copyable,
	// but not yet supported by gcc4.7.
	static_assert(boost::has_trivial_copy<Parameters>::value,
				  "Paramters should be trivially copyable");
};

} // namespace euter
