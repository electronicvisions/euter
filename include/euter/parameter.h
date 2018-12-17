#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/to_seq.h>

#include <boost/functional/hash.hpp>
#include "boost/serialization/serialization.h"

namespace euter {

// Dark magic happens here, my friend. Use as:
// PARAMETER_STRUCT(IF_Foo_Bar_Parameters,
//     PARAMETER(double, v_init, -80.0),
//     PARAMETER(double, v_reset, 80),
//     PARAMETER(std::vector<double>, spikes, {}))
//
// IF_Foo_Bar_Parameters p = IF_Foo_Bar_Parameters::getDefault();
// IF_Foo_Bar_Parameters::getNames() returns list of names

class InvalidParameter : public std::exception {
public:
	InvalidParameter() :
	    std::exception(), mMsg()
	{}

	InvalidParameter(std::string const & msg) :
	    std::exception(), mMsg(msg)
	{}

	virtual ~InvalidParameter() throw() {}

	virtual char const * what() const throw() { return mMsg.c_str(); }


private:
	const std::string mMsg;
};

} // namespace euter

// Structure holding paramter information (BOOST PP Sequence format)
// note value is "escaped" with an extra parenthese to support commas in
// initializer lists
#define PARAMETER(TYPE, NAME, VALUE) (TYPE)(NAME)((VALUE))

// Macro to define an initializer list for a paramter
#define PARAMETER_INITIALIZER_LIST(...) { __VA_ARGS__ }

// Helper that removes the extra parenthesis around VALUE
#define PARAMETER_REMOVE_PARENTHESES_I(...) __VA_ARGS__
#define PARAMETER_REMOVE_PARENTHESES(X) PARAMETER_REMOVE_PARENTHESES_I X

// Parameter asses macros
#define PARAMETER_GET_TYPE(PARAMETER_SEQ) BOOST_PP_SEQ_ELEM(0,PARAMETER_SEQ)
#define PARAMETER_GET_NAME(PARAMETER_SEQ) BOOST_PP_SEQ_ELEM(1,PARAMETER_SEQ)
#define PARAMETER_GET_NAME_STRING(PARAMETER_SEQ) BOOST_PP_STRINGIZE(PARAMETER_GET_NAME(PARAMETER_SEQ))
#define PARAMETER_GET_DEFAULT(PARAMETER_SEQ) \
	PARAMETER_REMOVE_PARENTHESES(BOOST_PP_SEQ_ELEM(2,PARAMETER_SEQ))

// Utility macros

// Checking for Parameter name e.g.:
// ( name == "v_init" )
#define PARAMETER_NAME_EQ(NAME, P) ( NAME == PARAMETER_GET_NAME_STRING(P) )

// Calls Visitor with a paramerter as argument
// v(p.v_init)
#define PARAMETER_VISITOR_CALL(VISITOR, P) \
	VISITOR ( PARAMETER_GET_NAME(P) );

#ifndef PYPLUSPLUS
#define PARAMETER_DEFAULT_VALUE_BODY(P) { return PARAMETER_GET_DEFAULT(P); }
#else
#define PARAMETER_DEFAULT_VALUE_BODY(P) ;
#endif

// Generates a static function returning the parameters default value
// -> static double v_init_default() { return -80.0; }
#define PARAMETER_DEFAULT_VALUE(P) \
	static PARAMETER_GET_TYPE(P) \
	BOOST_PP_CAT(PARAMETER_GET_NAME(P), _default ) () \
	PARAMETER_DEFAULT_VALUE_BODY(P)


// Functional macros

// Defines parameter type e.g. 'double v_init;'
#define PARAMETER_FUNC_DEFINE_MEMBER(r, DATA, P) \
	PARAMETER_GET_TYPE(P) PARAMETER_GET_NAME(P); \
    PARAMETER_DEFAULT_VALUE(P)

// Calls vistor if the name matches
// -> if ( name == "v_init" ) { v(p.v_init); } else
#define PARAMETER_FUNC_VISTOR_IF_ELSE(r, DATA, P) \
	if PARAMETER_NAME_EQ(__name, P) { PARAMETER_VISITOR_CALL(__v, P) } else

// Calls vistor for the named parameter
// -> v(p.v_init);
#define PARAMETER_FUNC_VISTOR_CALL(r, DATA, P) \
	PARAMETER_VISITOR_CALL(__v, P)

// Set default value
// -> p.v_init = -80.0
#define PARAMETER_FUNC_SET_DEFAULT(r, DATA, P) \
	p. PARAMETER_GET_NAME(P) = PARAMETER_GET_DEFAULT(P);

// Transforms the name so that it can be used in an initilizer list
// -> {"v_init"},;
#define PARAMETER_FUNC_NAME_INITILIZER(r, DATA, P) \
	{ PARAMETER_GET_NAME_STRING(P) },

// Apply combine_hash to the given parameter
// -> hash_combine(seed, PARAMTER_GET_NAME(P))
#ifndef PYPLUSPLUS
#define PARAMETER_FUNC_HASH(r, DATA, P) \
	boost::hash_combine(seed, PARAMETER_GET_NAME(P));
#else
#define PARAMETER_FUNC_HASH(r, DATA, P)
#endif

#define PARAMETER_FUNC_COMP(r, DATA, P) \
	&& (PARAMETER_GET_NAME(P) == other.PARAMETER_GET_NAME(P))

// Append the parameter to an archive
// -> & boost::serialization::make_nvp("v_init", p.v_init)
#define PARAMETER_FUNC_AR(r, DATA, P) \
	& boost::serialization::make_nvp(PARAMETER_GET_NAME_STRING(P), PARAMETER_GET_NAME(P))

// stream parameter name and value
// for ostream operator<<
// -> << "a" << ": " << obj.a << "\n"
#define PARAMETER_FUNC_OUT(r, OBJ, P) \
	<< PARAMETER_GET_NAME_STRING(P) << ": " << OBJ . PARAMETER_GET_NAME(P) << "\n"

////////////////////////////////////////////////////////////////////////////////
// Implenting class functionality

// Defines the getter for the PARAMETER_STRUCT
// void get(Name, Visitor()) [const]
#define PARAMETER_STRUCT_GETTER_IMPL(CONST, ...)                           \
template <typename Visitor>                                                \
void get(const std::string & __name, const Visitor & __v) CONST {          \
    BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_VISTOR_IF_ELSE, ~,                \
                          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))           \
    {                                                                      \
        std::string err("No such parameter '");                            \
        err += __name + "'";                                               \
        throw euter::InvalidParameter(err);                                \
    }                                                                      \
}                                                                          \

// Defines the all vistor for the PARAMETER_STRUCT
// void all(Visitor()) [const]
#define PARAMETER_STRUCT_ALL_IMPL(CONST, ...)                                  \
template <typename Visitor>                                                    \
void all(const Visitor & __v) CONST {                                          \
    BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_VISTOR_CALL, ~,                       \
                          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))               \
}

#ifndef PYPLUSPLUS
#define PARAMETER_STRUCT_STATICS_DEFAULTS(CLASS_NAME, ...) {                   \
        CLASS_NAME p;                                                          \
        BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_SET_DEFAULT, &,                   \
                              BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));          \
        return p;                                                              \
    }
#define PARAMETER_STRUCT_STATICS_NAMES(CLASS_NAME, ...) {                      \
        static const std::vector<std::string> names = {                        \
            BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_NAME_INITILIZER, &,           \
                                  BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) };    \
        return names;                                                          \
    }
#else
#define PARAMETER_STRUCT_STATICS_DEFAULTS(CLASS_NAME, ...) ;
#define PARAMETER_STRUCT_STATICS_NAMES(CLASS_NAME, ...) ;
#endif

/** Construct a Parameter with default values set */
// getDefault
#define PARAMETER_STRUCT_STATICS(CLASS_NAME, ...)                              \
    static CLASS_NAME getDefault()                                             \
		PARAMETER_STRUCT_STATICS_DEFAULTS(CLASS_NAME, __VA_ARGS__)             \
    /** Returns a vector containing all parameter names */                     \
    static const std::vector<std::string> & getNames()                         \
		PARAMETER_STRUCT_STATICS_NAMES(CLASS_NAME, __VA_ARGS__)

#define PARAMETER_STRUCT_GETTER(...)                                           \
    PARAMETER_STRUCT_GETTER_IMPL(BOOST_PP_EMPTY(), __VA_ARGS__)                \
    PARAMETER_STRUCT_GETTER_IMPL(const, __VA_ARGS__)                           \

#define PARAMETER_STRUCT_ALL(...)                                              \
    PARAMETER_STRUCT_ALL_IMPL(BOOST_PP_EMPTY(), __VA_ARGS__)                   \
    PARAMETER_STRUCT_ALL_IMPL(const, __VA_ARGS__)                              \

#define PARAMETER_STRUCT_MEMBERS(...)                                          \
    BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_DEFINE_MEMBER, ~,                     \
                          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))               \

#define PARAMETER_STRUCT_MEMBER_INITIALIZER(r, DATA, P)                        \
    ( PARAMETER_GET_NAME(P) ( PARAMETER_GET_DEFAULT(P) ) )

#ifndef PYPLUSPLUS
#define PARAMETER_STRUCT_MEMBER_DEFAULTS(CLASS_NAME, ...)                      \
   CLASS_NAME() :                                                              \
        BOOST_PP_SEQ_ENUM(                                                     \
            BOOST_PP_SEQ_FOR_EACH(PARAMETER_STRUCT_MEMBER_INITIALIZER, ~,      \
                              BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))          \
        {}
#else
#define PARAMETER_STRUCT_MEMBER_DEFAULTS(CLASS_NAME, ...)                      \
   CLASS_NAME() {}
#endif // PYPLUSPLUS



// Defines the paramter struct
			/* ..calculate hash value for t */
#define PARAMETER_STRUCT(CLASS_NAME, ...)                                      \
struct CLASS_NAME {                                                            \
    PARAMETER_STRUCT_MEMBER_DEFAULTS(CLASS_NAME, __VA_ARGS__)                  \
    PARAMETER_STRUCT_MEMBERS(__VA_ARGS__)                                      \
    PARAMETER_STRUCT_GETTER(__VA_ARGS__)                                       \
    PARAMETER_STRUCT_ALL(__VA_ARGS__)                                          \
    PARAMETER_STRUCT_STATICS(CLASS_NAME, __VA_ARGS__)                          \
    \
	size_t hash() const {                                                      \
		size_t seed = 0;                                                       \
        BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_HASH, &,                          \
			BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                             \
		return seed;                                                           \
	}                                                                          \
	\
    /** boost serialization support */                                         \
    bool operator==(CLASS_NAME const& other) const {                           \
        return true BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_COMP, &,                          \
			BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));                            \
		return true; 														   \
    }                                                                          \
	\
    /** boost serialization support */                                         \
    template <typename Archiver>                                               \
    void serialize(Archiver& ar, const unsigned int&){                         \
        ar BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_AR, &,                         \
                                 BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));       \
    }                                                                          \
                                                                               \
	friend std::ostream& operator<<(std::ostream& out, CLASS_NAME const& obj)  \
	{                                                                          \
		out << BOOST_PP_STRINGIZE(CLASS_NAME) << ":\n";                        \
        out BOOST_PP_SEQ_FOR_EACH(PARAMETER_FUNC_OUT, obj,                     \
                                 BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));       \
		return out;                                                            \
	}                                                                          \
};                                                                             \

namespace euter {

struct DefaultVisitor
{
	template <typename Parameter>
	void operator()(Parameter const& /*p*/) const {}
};

} // namespace euter
