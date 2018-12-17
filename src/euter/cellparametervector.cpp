#include "euter/cellparametervector.h"

#include "boost/make_shared.hpp"

#include "euter/typedcellparametervector.h"

namespace euter {

namespace
{

struct CallCreate
{
	typedef std::unique_ptr<CellParameterVector> return_value;

	CallCreate(size_t size) : size(size) {}

	template <CellType T>
	return_value apply() const
	{
		return std::unique_ptr<CellParameterVector>(
		            new TypedCellParameterVector<T>(size));
	}

	size_t size;
};

struct DoCopy
{
	typedef std::unique_ptr<CellParameterVector> return_value;

	DoCopy(const CellParameterVector * vector) : vector(vector) {}

	template <CellType T>
	return_value apply() const
	{
		typedef const TypedCellParameterVector<T> TypedVector;
		TypedVector * v = static_cast<TypedVector *>(vector);
		return std::unique_ptr<CellParameterVector>(
		            new TypedCellParameterVector<T>(*v));
	}

	const CellParameterVector * vector;
};

} // end namespace

CellParameterVector::~CellParameterVector() {}

CellParameterVector::CellParameterVector() {}

CellParameterVector::CellParameterVector(size_t size) : mPositions(size)
{

}

std::unique_ptr<CellParameterVector> CellParameterVector::create(size_t size, CellType type)
{
	return visitCellTypes(type, CallCreate(size));
}

std::unique_ptr<CellParameterVector> CellParameterVector::copy() const
{
	return visitCellTypes(type(), DoCopy(this));
}

std::vector<Point3d> & CellParameterVector::positions()
{
	return mPositions;
}

const std::vector<Point3d> & CellParameterVector::positions() const
{
	return mPositions;
}

} // namespace euter

BOOST_CLASS_EXPORT_IMPLEMENT(euter::CellParameterVector)
