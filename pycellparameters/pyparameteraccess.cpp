#include "pyparameteraccess.h"

#include "getparametervector.h"
#include "euter/population.h"
#include "euter/population_view.h"


ParameterProxy getPyParameter(PopulationView & view, size_t index)
{
	CellParameterVector& p = view.population().parameters();
	return visitCellParameterVector(p, GetParameterProxy(), index);
}

const ParameterProxy getPyParameter(const PopulationView & view, size_t index)
{
	CellParameterVector const& p = view.population().parameters();
	return visitCellParameterVector(p, GetParameterProxy(), index);
}

std::vector<ParameterProxy> getPyParameterVector(PopulationView & view)
{
	CellParameterVector& p = view.population().parameters();
	if(view.mask().empty())
	{
		return visitCellParameterVector(p, GetParameterProxyVector());
	}
	else
	{
		return visitCellParameterVector(p, GetParameterProxyVector(), std::cref(view.mask()));
	}
}

const std::vector<ParameterProxy> getPyParameterVector(const PopulationView & view)
{
	CellParameterVector const& p = view.population().parameters();
	if(view.mask().empty())
	{
		return visitCellParameterVector(p, GetParameterProxyVector());
	}
	else
	{
		return visitCellParameterVector(p, GetParameterProxyVector(), std::cref(view.mask()));
	}
}
