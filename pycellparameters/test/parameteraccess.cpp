
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "euter/celltypes.h"
#include "euter/typedcellparametervector.h"
#include "pycellparameters/getparametervector.h"
#include "pycellparameters/pyparameteraccess.h"

namespace bp = boost::python;

namespace
{
struct SetValue : DefaultVisitor
{
	SetValue(double v) : v(v) {}

	void operator()(double & p) const
	{
		p = v;
	}

	void operator()(bool & p) const
	{
		p = bool(v);
	}

	double v;
};

struct SetSomething : DefaultVisitor
{
	SetSomething() : v(3.2) {}

	void operator()(double & p) const
	{
		p = v;
		v *= 1.7;
		v -= 1.0;
	}

	void operator()(bool & p) const
	{
		p = true;
	}

	mutable double v;
};

struct GetDoubleValue
{
	GetDoubleValue(double & obj) : obj(obj) {}

	void operator()(const double & p) const
	{
		obj = p;
	}

	template <typename Parameter>
	void operator()(const Parameter &) const
	{
		FAIL() << "Parameter is not double.";
	}

	double & obj;
};

template <typename Parameters>
double getValue(Parameters & p, const std::string & name)
{
	double ret;
	p.get(name, GetDoubleValue(ret));
	return ret;
}

}

TEST(Parameters, Set_EIF_cond_exp_isfa_ista_Parameters)
{
	typedef CellTypeTraits<CellType::EIF_cond_exp_isfa_ista>::Parameters P;
	P a = P::getDefault();

	ASSERT_EQ(4.0, getValue(a, "a"));
	ASSERT_EQ(9.3667, getValue(a, "tau_m"));
	ASSERT_EQ(0.0, getValue(a, "e_rev_E"));
	ASSERT_EQ(0.0, getValue(a, "i_offset"));
	ASSERT_EQ(0.281, getValue(a, "cm"));

	a.all(SetSomething());

	ASSERT_EQ(a.tau_refrac, getValue(a, "tau_refrac"));
	ASSERT_EQ(a.a, getValue(a, "a"));
	ASSERT_EQ(a.tau_m, getValue(a, "tau_m"));
	ASSERT_EQ(a.e_rev_E, getValue(a, "e_rev_E"));
	ASSERT_EQ(a.i_offset, getValue(a, "i_offset"));
	ASSERT_EQ(a.cm, getValue(a, "cm"));
	ASSERT_EQ(a.delta_T, getValue(a, "delta_T"));
	ASSERT_EQ(a.e_rev_I, getValue(a, "e_rev_I"));
	ASSERT_EQ(a.v_thresh, getValue(a, "v_thresh"));
	ASSERT_EQ(a.b, getValue(a, "b"));
	ASSERT_EQ(a.tau_syn_E, getValue(a, "tau_syn_E"));
	ASSERT_EQ(a.v_reset, getValue(a, "v_reset"));
	ASSERT_EQ(a.v_spike, getValue(a, "v_spike"));
	ASSERT_EQ(a.v, getValue(a, "v"));
	ASSERT_EQ(a.tau_syn_I, getValue(a, "tau_syn_I"));
	ASSERT_EQ(a.tau_w, getValue(a, "tau_w"));
	ASSERT_EQ(a.w, getValue(a, "w"));
	ASSERT_EQ(a.v_rest, getValue(a, "v_rest"));

	ASSERT_EQ(P::getNames().size(), 22);
}

TEST(Parameters, Serialize_EIF_cond_exp_isfa_ista_Parameters)
{
	typedef CellTypeTraits<CellType::EIF_cond_exp_isfa_ista>::Parameters P;
	P a = P::getDefault();
	P b = P::getDefault();

	a.v = 4.0;
	a.get("v_reset", SetValue(-80.0));

	std::stringstream tmp;
	boost::archive::xml_oarchive oa(tmp);
	oa << boost::serialization::make_nvp("object", a);
	oa << boost::serialization::make_nvp("object", b);

	a.v = 3.0;
	a.get("v_reset", SetValue(5.0));

	boost::archive::xml_iarchive ia(tmp);
	ia >> boost::serialization::make_nvp("object", a);
	ia >> boost::serialization::make_nvp("object", b);

	ASSERT_EQ(4.0, getValue(a, "a"));
	ASSERT_EQ(a.v_reset, -80.0);

	ASSERT_EQ(9.3667, getValue(a, "tau_m"));
	ASSERT_EQ(0.0, getValue(a, "e_rev_E"));

	//ASSERT_EQ(b, P::getDefault());
}

TEST(Parameters, ParameterProxy)
{
	typedef CellTypeTraits<CellType::EIF_cond_exp_isfa_ista>::Parameters P;
	{
		double value = 5.0;
		P p = P::getDefault();
		auto proxy = getParameterProxy(p);
		proxy.set("v_reset", bp::object(value));
		ASSERT_EQ(p.v_reset, value);
		bp::object read_value;
		ASSERT_NO_THROW(proxy.get("v_reset", read_value));
		ASSERT_EQ(value, bp::extract<double>(read_value));
	}
	{
		const P p(P::getDefault());
		auto proxy = getParameterProxy(p);
		bp::object read_value;
		ASSERT_NO_THROW(proxy.get("v_reset", read_value));
		ASSERT_EQ(P::getDefault().v_reset, bp::extract<double>(read_value));
		ASSERT_THROW(proxy.set("v_reset", bp::object(5.0)),
					 std::runtime_error);
	}
}

namespace
{
struct CompareAddress
{
	typedef bool return_type;

	template <typename TypedCellParameterVector>
	return_type operator()(TypedCellParameterVector & v, void * other) const
	{
		return &(v) == other;
	}
};

struct IsConst 
{
	typedef bool return_type;

	template <typename TypedCellParameterVector>
	return_type operator()(TypedCellParameterVector & ) const
	{
		return std::is_const<TypedCellParameterVector>::value;
	}
};
}

TEST(Parameters, PopulationViewUtil)
{
	constexpr CellType type = CellType::EIF_cond_exp_isfa_ista;

	std::unique_ptr<CellParameterVector> p = CellParameterVector::create(11, type);
	
	ASSERT_TRUE(visitCellParameterVector(*p, CompareAddress(), p.get()));	
	ASSERT_FALSE(visitCellParameterVector(*p, IsConst()));	

	const CellParameterVector& p_const = const_cast<const CellParameterVector&>(*p);
	ASSERT_TRUE(visitCellParameterVector(p_const, IsConst()));


	ParameterProxy a = visitCellParameterVector(*p, GetParameterProxy(), 2);
	ParameterProxy b = visitCellParameterVector(*p, GetParameterProxy(), 2);

	bp::object read_value;
	a.set("v_reset", bp::object(3.0));
	b.get("v_reset", read_value);
	ASSERT_EQ(3.0, bp::extract<double>(read_value));
}
