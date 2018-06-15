#pragma once

#include <stddef.h>

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.h>

////////////////////////////////////////////////////////////////////////////////
//
// This header defines the CellType Enum and utili datatypes and functions
// To add an celltype just add it to the CELL_TYPE_SEQ macro, the rest is then
// automagic

#define CELL_TYPE_SEQ BOOST_PP_VARIADIC_TO_SEQ(\
EIF_cond_alpha_isfa_ista, EIF_cond_exp_isfa_ista, \
HH_cond_exp, IF_cond_alpha, IF_cond_exp, IF_cond_exp_gsfa_grr, IF_curr_alpha, \
IF_curr_exp, IF_facets_hardware1, IF_brainscales_hardware, \
SpikeSourceArray, SpikeSourceInhGamma, SpikeSourcePoisson, \
IF_multicond_exp, EIF_multicond_exp_isfa_ista)

enum class CellType : size_t
{
	BOOST_PP_SEQ_ENUM(CELL_TYPE_SEQ)
};

std::string getCellTypeName(CellType t);

extern const std::map<std::string, CellType> nameToCellType;


#define CELL_TYPE_VISITOR_CALL(r, VISITOR, CELLTYPE) \
	VISITOR .template apply< CellType:: CELLTYPE >();

#define CELL_TYPE_CASE_STATMENT(r, VISITOR, CELLTYPE) \
case CellType:: CELLTYPE : \
	return CELL_TYPE_VISITOR_CALL(r, VISITOR, CELLTYPE)

template <class Visitor>
typename Visitor::return_value visitCellTypes(CellType type, const Visitor & v)
{
	switch(type)
	{
	BOOST_PP_SEQ_FOR_EACH(CELL_TYPE_CASE_STATMENT, v, CELL_TYPE_SEQ);
	}
	throw std::runtime_error("Invalid CellType, go and hit the developers...");
}

template <class Visitor>
typename Visitor::return_value forallCellTypes(const Visitor & v)
{
	BOOST_PP_SEQ_FOR_EACH(CELL_TYPE_VISITOR_CALL, v, CELL_TYPE_SEQ);
}

#undef CELL_TYPE_VISITOR_CALL
#undef CELL_TYPE_CASE_STATMENT

struct DefaultCellTypeVisitor
{
	typedef void return_value;

	template <CellType T>
	return_value apply() const {}
};

template <typename Archiver, template <CellType> class Type>
struct RegisterCellTypeForSerializationVisitor
{
	typedef void return_value;

	RegisterCellTypeForSerializationVisitor(Archiver & ar) : ar(ar) {}

	template <CellType T>
	return_value apply() const
	{
		ar.template register_type< Type<T> >();
	}

	Archiver & ar;
};

#include "celltypes_traits.h"
