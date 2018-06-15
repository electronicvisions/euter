#pragma once

#include "boost/serialization/serialization.h"
#ifndef PYPLUSPLUS
#include "boost/serialization/unique_ptr.h"
#endif
#include "celltypes.h"
#include "point.h"

/// Bass class for holding a vector of cell parameters
/// @note If you want to serialize this class you have to do it manually, because
/// the derivated classes are templates. In this case the BOOST_CLASS_EXPORT_..
/// macros can not be used. For an example see Population::serialize
class CellParameterVector
{
public:
	virtual ~CellParameterVector();
	CellParameterVector();
	CellParameterVector(size_t size);

	virtual size_t size() const = 0;
	virtual CellType type() const = 0;
	virtual bool is_source() const = 0;

	std::vector<Point3d> & positions();
	const std::vector<Point3d> & positions() const;

#ifndef PYPLUSPLUS
	std::unique_ptr<CellParameterVector> copy() const;
	static std::unique_ptr<CellParameterVector> create(size_t size, CellType t);
#endif

private:
	std::vector<Point3d> mPositions;

	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int & version);

	friend class boost::serialization::access;
};

template<typename Archiver>
void CellParameterVector::serialize(Archiver & ar, const unsigned int & )
{
	ar & BOOST_SERIALIZATION_NVP(mPositions);
}

//BOOST_SERIALIZATION_FACTORY_1(CellParameterVector, size_t)
BOOST_SERIALIZATION_ASSUME_ABSTRACT(CellParameterVector)
BOOST_CLASS_EXPORT_KEY(CellParameterVector)
