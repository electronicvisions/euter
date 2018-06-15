#pragma once

#include "cellparametervector.h"

template <CellType _Type>
class TypedCellParameterVector : public CellParameterVector
{
public:
	static const CellType Type = _Type;
	typedef typename CellTypeTraits<Type>::Parameters CellParameters;

	TypedCellParameterVector() {}
	TypedCellParameterVector(size_t size) :
	    CellParameterVector(size),
	    mCellparams(size, CellParameters::getDefault()) {}

	virtual size_t size() const
	{ return mCellparams.size(); }

	virtual CellType type() const
	{ return Type; }

	virtual bool is_source() const
	{ return CellTypeTraits<Type>::is_source; }

	std::vector<CellParameters> & parameters()
	{ return mCellparams; }

	const std::vector<CellParameters> & parameters() const
	{ return mCellparams; }
private:
	std::vector<CellParameters> mCellparams;

	template<typename Archiver>
	void serialize(Archiver& ar, const unsigned int & version);

	friend class boost::serialization::access;
};

template <CellType Type>
template<typename Archiver>
void TypedCellParameterVector<Type>::serialize(Archiver& ar, const unsigned int & )
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CellParameterVector)
	   & BOOST_SERIALIZATION_NVP(mCellparams);
}

////
// Using Boost PP to generate a switchcase determine the vector type
#define DO_VISITOR_CALL(r, TYPE, CELLTYPE) \
case CellType:: CELLTYPE : \
	return v(static_cast< TYPE <CellType:: CELLTYPE > &>(p), std::forward<Args>(args)...);

template <typename Visitor, typename ... Args>
typename Visitor::return_type
visitCellParameterVector(CellParameterVector & p, Visitor& v, Args && ... args)
{
	switch(p.type())
	{
	BOOST_PP_SEQ_FOR_EACH(DO_VISITOR_CALL, TypedCellParameterVector, CELL_TYPE_SEQ);
	}
	throw std::runtime_error("Invalid CellType, go and hit the developers...");
}

template <typename Visitor, typename ... Args>
typename Visitor::return_type
visitCellParameterVector(const CellParameterVector & p, Visitor& v, Args && ... args)
{
	switch(p.type())
	{
	BOOST_PP_SEQ_FOR_EACH(DO_VISITOR_CALL, const TypedCellParameterVector, CELL_TYPE_SEQ);
	}
	throw std::runtime_error("Invalid CellType, go and hit the developers...");
}

template <typename Visitor, typename ... Args>
typename Visitor::return_type
visitCellParameterVector(CellParameterVector & p, Visitor const& v, Args && ... args)
{
	switch(p.type())
	{
	BOOST_PP_SEQ_FOR_EACH(DO_VISITOR_CALL, TypedCellParameterVector, CELL_TYPE_SEQ);
	}
	throw std::runtime_error("Invalid CellType, go and hit the developers...");
}

template <typename Visitor, typename ... Args>
typename Visitor::return_type
visitCellParameterVector(const CellParameterVector & p, Visitor const& v, Args && ... args)
{
	switch(p.type())
	{
	BOOST_PP_SEQ_FOR_EACH(DO_VISITOR_CALL, const TypedCellParameterVector, CELL_TYPE_SEQ);
	}
	throw std::runtime_error("Invalid CellType, go and hit the developers...");
}

#undef DO_VISITOR_CALL
