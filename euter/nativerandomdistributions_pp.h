#pragma once

#ifdef INIT
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/variadic/to_seq.h>

#define F_DECL(s, data, elem) BOOST_PP_SEQ_ELEM(0, elem) BOOST_PP_SEQ_ELEM(1, elem) = BOOST_PP_SEQ_ELEM(2, elem)
#define F_TYPE(s, data, elem) BOOST_PP_SEQ_ELEM(0, elem)
#define F_NAME(s, data, elem) BOOST_PP_SEQ_ELEM(1, elem)

#define GET_DECLARATIONS(...) \
	    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(F_DECL, 0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))
#define GET_NAMES(...) \
	    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(F_NAME, 0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))
#define GET_TYPES(...) \
	    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(F_TYPE, 0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#ifdef NATIVE_RANDOM_DISTRIBUTION_DO_EXPORT_IMPL
	#define DO_EXPORT(NAME) BOOST_CLASS_EXPORT_KEY(NAME) BOOST_CLASS_EXPORT_IMPLEMENT(NAME)
#else
	#define DO_EXPORT(NAME) BOOST_CLASS_EXPORT_KEY(NAME)
#endif

#define NATIVE_DISTRIBUTION(NAME, TYPE, RETURN_TYPE, ...)                   \
struct NAME : NativeRandomDistributionImpl< TYPE <RETURN_TYPE> >            \
{                                                                           \
public:                                                                     \
	typedef NativeRandomDistributionImpl< TYPE <RETURN_TYPE> > base_type;   \
	template<template<typename...> class Target, typename ... Args>         \
	struct GetArgumentTypes{                                                \
		typedef Target<Args..., GET_TYPES(__VA_ARGS__)> type;               \
	}; /* Use as GetArgumentTypes<std::tuple>::type */                      \
\
	NAME (const rng_type & rng, GET_DECLARATIONS(__VA_ARGS__)) :            \
	    base_type(rng, dist_type(GET_NAMES(__VA_ARGS__))) {}                \
\
	virtual RandomDistribution * save() const { return new NAME(*this); }   \
\
private:                                                                    \
	NAME() {}                                                               \
	template<typename Archiver>                                             \
	void serialize(Archiver& ar, const unsigned int &) 	{                   \
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_type);                \
	}                                                                       \
	friend class boost::serialization::access;                              \
};                                                                          \
DO_EXPORT(NAME)

#endif


#ifdef CLEANUP

#undef F_DECL
#undef F_NAME
#undef F_TYPE
#undef GET_DECLARATIONS
#undef GET_NAMES
#undef GET_TYPES
#undef DO_EXPORT
#undef NATIVE_DISTRIBUTION
#undef DISTRIBUTION_IMPL
#undef DISTRIBUTION_SERIALIZE

#endif


