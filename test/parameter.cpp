#include <gtest/gtest.h>

#include "euter/celltypes_traits.h"

using namespace euter;
using namespace PyNNParameters;

TEST(Parameter, EIF_cond_exp_isfa_ista_DEFAULT)
{
	EIF_cond_exp_isfa_ista nrn;
	ASSERT_EQ(0.1, nrn.tau_refrac);
	ASSERT_EQ(4.0, nrn.a);
	ASSERT_EQ(9.3667, nrn.tau_m);
	ASSERT_EQ(0.0, nrn.e_rev_E);
	ASSERT_EQ(0.0, nrn.i_offset);
	ASSERT_EQ(0.281, nrn.cm);
	ASSERT_EQ(2.0, nrn.delta_T);
	ASSERT_EQ(-80.0, nrn.e_rev_I);
	ASSERT_EQ(-50.4, nrn.v_thresh);
	ASSERT_EQ(0.0805, nrn.b);
	ASSERT_EQ(5.0, nrn.tau_syn_E);
	ASSERT_EQ(-70.6, nrn.v_reset);
	ASSERT_EQ(-40.0, nrn.v_spike);
	ASSERT_EQ(-65.0, nrn.v);
	ASSERT_EQ(5.0, nrn.tau_syn_I);
	ASSERT_EQ(144.0, nrn.tau_w);
	ASSERT_EQ(0.0, nrn.w);
	ASSERT_EQ(-70.6, nrn.v_rest);

	ASSERT_FALSE(nrn.record_spikes);
	ASSERT_FALSE(nrn.record_v);
	ASSERT_FALSE(nrn.record_w);
	ASSERT_FALSE(nrn.record_gsyn);

	ASSERT_TRUE(boost::has_trivial_copy<EIF_cond_exp_isfa_ista>::value);
}

TEST(Parameter, IF_cond_exp_DEFAULT)
{
	IF_cond_exp nrn;
	ASSERT_EQ(0.1, nrn.tau_refrac);
	ASSERT_EQ(1.0, nrn.cm);
	ASSERT_EQ(5.0, nrn.tau_syn_E);
	ASSERT_EQ(-65.0, nrn.v_rest);
	ASSERT_EQ(5.0, nrn.tau_syn_I);
	ASSERT_EQ(20.0, nrn.tau_m);
	ASSERT_EQ(0.0, nrn.e_rev_E);
	ASSERT_EQ(0.0, nrn.i_offset);
	ASSERT_EQ(-70.0, nrn.e_rev_I);
	ASSERT_EQ(-50.0, nrn.v_thresh);
	ASSERT_EQ(-65.0, nrn.v_reset);
	ASSERT_EQ(-65.0, nrn.v);

	ASSERT_FALSE(nrn.record_spikes);
	ASSERT_FALSE(nrn.record_v);
	ASSERT_FALSE(nrn.record_gsyn);

	ASSERT_TRUE(boost::has_trivial_copy<IF_cond_exp>::value);
}
